
#include "Share.fx"

StructuredBuffer<matrix> g_BoneMatrixArray : register(t11);
StructuredBuffer<matrix> g_InstancingBoneMatrixArray : register(t12);

struct VS_INPUT_3D
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float4 BlendWeight : BLENDWEIGHT;
    float4 BlendIndex : BLENDINDEX;
};

struct VS_OUTPUT_3D
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 ProjPos : POSITION;
    float3 ViewPos : POSITION1;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
};

struct SkinningInfo
{
    float3  Pos;
    float3  Normal;
    float3  Tangent;
    float3  Binormal;
};

SkinningInfo Skinning(float3 Pos, float3 Normal, float3 Tangent, float3 Binormal, float4 BlendWeight, float4 BlendIndex)
{
    SkinningInfo Info = (SkinningInfo)0;

    for (int i = 0; i < 4; i++)
    {
        if (BlendWeight[i] == 0.f)
        {
            continue;
        }

        matrix  matBone = g_BoneMatrixArray[(int)BlendIndex[i]];

        Info.Pos += (mul(float4(Pos, 1.f), matBone) * BlendWeight[i]).xyz;
        Info.Normal += (mul(float4(Normal, 0.f), matBone) * BlendWeight[i]).xyz;
        Info.Tangent += (mul(float4(Tangent, 0.f), matBone) * BlendWeight[i]).xyz;
        Info.Binormal += (mul(float4(Binormal, 0.f), matBone) * BlendWeight[i]).xyz;
    }

    Info.Normal = normalize(Info.Normal);
    Info.Tangent = normalize(Info.Tangent);
    Info.Binormal = normalize(Info.Binormal);

    return Info;
}

VS_OUTPUT_3D MeshVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D)0;

    SkinningInfo Info;
    Info.Pos = input.Pos;
    Info.Normal = input.Normal;
    Info.Tangent = input.Tangent;
    Info.Binormal = input.Binormal;

    if (g_MtrlAnimation3D == 1)
    {
        Info = Skinning(input.Pos, input.Normal, input.Tangent, input.Binormal, input.BlendWeight, input.BlendIndex);
    }

    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.ProjPos = mul(float4(Info.Pos, 1.f), g_matWVP);

    output.ViewPos = mul(float4(Info.Pos, 1.f), g_matWV).xyz;

    output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_matWV).xyz);
    output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_matWV).xyz);
    output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_matWV).xyz);

    output.Pos = output.ProjPos;
    output.UV = input.UV;

    return output;
}

PS_OUTPUT_GBUFFER MeshPS(VS_OUTPUT_3D input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER)0;

    float4  TextureColor = g_BaseTexture.Sample(g_LinearSmp, input.UV);

    if (TextureColor.a == 0.f || g_MtrlOpacity == 0.f)
    {
        clip(-1);
    }

    output.GBuffer1.rgb = TextureColor.rgb;
    output.GBuffer1.a = TextureColor.a * g_MtrlOpacity;

    output.GBuffer2.rgb = ComputeBumpNormal(input.Tangent, input.Binormal, input.Normal, input.UV);
    output.GBuffer2.a = 1.f;

    // 원근투영을 이용하여 변환된 투영공간의 위치에서 w는 뷰공간에서의
    // z값이 그대로 들어간다.
    output.GBuffer3.r = input.ProjPos.z / input.ProjPos.w;
    output.GBuffer3.g = input.ProjPos.w;
    output.GBuffer3.b = g_MtrlSpecularColor.w;
    output.GBuffer3.a = g_MtrlReceiveDecal;

    output.GBuffer5.a = 1.f;
    output.GBuffer5.rgb = input.Tangent;

    output.GBuffer6.a = 1.f;
    output.GBuffer6.rgb = input.Normal;

    output.GBuffer4.r = ConvertColor(g_MtrlBaseColor);
    output.GBuffer4.g = ConvertColor(g_MtrlAmbientColor);

    float4  SpecularColor = g_MtrlSpecularColor;

    if (g_MtrlBaseColor.a == 1.f)
    {
        SpecularColor.rgb = g_SpecularTexture.Sample(g_LinearSmp, input.UV).rrr;
    }

    output.GBuffer4.b = ConvertColor(SpecularColor);

    float4  EmissiveColor = g_MtrlEmissiveColor;

    if (g_MtrlEmissiveColor.a == 1.f)
    {
        EmissiveColor.rgb = g_EmissiveTexture.Sample(g_LinearSmp, input.UV).rgb;
    }

    output.GBuffer4.a = ConvertColor(EmissiveColor);

    return output;
}

struct VS_INPUT_INSTANCING3D
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float4 BlendWeight : BLENDWEIGHT;
    float4 BlendIndex : BLENDINDEX;
    uint InstanceID : SV_InstanceID;
};

struct VS_OUTPUT_INSTANCING3D
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 ProjPos : POSITION;
    float3 ViewPos : POSITION1;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    uint    InstanceID : TEXCOORD1;
};

SkinningInfo SkinningInstancing(float3 Pos, float3 Normal, float3 Tangent, float3 Binormal, float4 BlendWeight, float4 BlendIndex,
    int InstanceID)
{
    SkinningInfo Info = (SkinningInfo)0;

    for (int i = 0; i < 4; ++i)
    {
        if (BlendWeight[i] == 0.f)
        {
            continue;
        }

        matrix  matBone = g_InstancingBoneMatrixArray[InstanceID * g_InstancingBoneCount + (int)BlendIndex[i]];

        Info.Pos += (mul(float4(Pos, 1.f), matBone) * BlendWeight[i]).xyz;
        Info.Normal += (mul(float4(Normal, 0.f), matBone) * BlendWeight[i]).xyz;
        Info.Tangent += (mul(float4(Tangent, 0.f), matBone) * BlendWeight[i]).xyz;
        Info.Binormal += (mul(float4(Binormal, 0.f), matBone) * BlendWeight[i]).xyz;
    }

    Info.Normal = normalize(Info.Normal);
    Info.Tangent = normalize(Info.Tangent);
    Info.Binormal = normalize(Info.Binormal);

    return Info;
}

VS_OUTPUT_INSTANCING3D MeshInstancingVS(VS_INPUT_INSTANCING3D input)
{
    VS_OUTPUT_INSTANCING3D output = (VS_OUTPUT_INSTANCING3D)0;

    SkinningInfo Info;
    Info.Pos = input.Pos;
    Info.Normal = input.Normal;
    Info.Tangent = input.Tangent;
    Info.Binormal = input.Binormal;

    if (g_InstancingInfoArray[input.InstanceID].Animation3D == 1)
    {
        Info = SkinningInstancing(input.Pos, input.Normal, input.Tangent, input.Binormal, input.BlendWeight, input.BlendIndex, input.InstanceID);
    }

    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.ProjPos = mul(float4(Info.Pos, 1.f), g_InstancingInfoArray[input.InstanceID].matWVP);

    output.ViewPos = mul(float4(Info.Pos, 1.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz;

    output.Normal = normalize(mul(float4(Info.Normal, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
    output.Tangent = normalize(mul(float4(Info.Tangent, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);
    output.Binormal = normalize(mul(float4(Info.Binormal, 0.f), g_InstancingInfoArray[input.InstanceID].matWV).xyz);

    output.Pos = output.ProjPos;
    output.UV = input.UV;
    output.InstanceID = input.InstanceID;

    return output;
}

PS_OUTPUT_GBUFFER MeshInstancingPS(VS_OUTPUT_INSTANCING3D input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER)0;

    float4  TextureColor = g_BaseTexture.Sample(g_LinearSmp, input.UV);

    if (TextureColor.a == 0.f || g_InstancingInfoArray[input.InstanceID].Opacity == 0.f)
    {
        clip(-1);
    }

    output.GBuffer1.rgb = TextureColor.rgb;
    output.GBuffer1.a = TextureColor.a * g_InstancingInfoArray[input.InstanceID].Opacity;

    output.GBuffer2.rgb = ComputeBumpNormalInstancing(input.Tangent, input.Binormal, input.Normal, input.UV, input.InstanceID);
    output.GBuffer2.a = 1.f;

    // 원근투영을 이용하여 변환된 투영공간의 위치에서 w는 뷰공간에서의
    // z값이 그대로 들어간다.
    output.GBuffer3.r = input.ProjPos.z / input.ProjPos.w;
    output.GBuffer3.g = input.ProjPos.w;
    output.GBuffer3.b = g_InstancingInfoArray[input.InstanceID].SpecularColor.w;
    output.GBuffer3.a = g_InstancingInfoArray[input.InstanceID].ReceiveDecal;

    output.GBuffer5.a = 1.f;
    output.GBuffer5.rgb = input.Tangent;

    output.GBuffer6.rgb = input.Normal;
    output.GBuffer6.a = 1.f;

    output.GBuffer4.r = ConvertColor(g_InstancingInfoArray[input.InstanceID].BaseColor);
    output.GBuffer4.g = ConvertColor(g_InstancingInfoArray[input.InstanceID].AmbientColor);

    float4  SpecularColor = g_InstancingInfoArray[input.InstanceID].SpecularColor;

    if (g_InstancingInfoArray[input.InstanceID].BaseColor.a == 1.f)
    {
        SpecularColor.rgb = g_SpecularTexture.Sample(g_LinearSmp, input.UV).rrr;
    }

    output.GBuffer4.b = ConvertColor(SpecularColor);

    float4  EmissiveColor = g_InstancingInfoArray[input.InstanceID].EmissiveColor;

    if (g_InstancingInfoArray[input.InstanceID].EmissiveColor.a == 1.f)
    {
        EmissiveColor.rgb = g_EmissiveTexture.Sample(g_LinearSmp, input.UV).rgb;
    }

    output.GBuffer4.a = ConvertColor(EmissiveColor);

    return output;
}
