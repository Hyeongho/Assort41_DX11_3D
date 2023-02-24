#include "Share.fx"

StructuredBuffer<matrix> g_BoneMatrixArray : register(t11);
StructuredBuffer<matrix> g_InstancingBoneMatrixArray : register(t12);

struct VS_OUTPUT_SHADOW
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
};

struct VS_OUTPUT_SHADOWINSTANCE
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
    uint InstanceID : TEXCOORD;
};

float3 Skinning(float3 Pos, float4 BlendWeight, float4 BlendIndex)
{
    float3 Info = (float3)0;

    for (int i = 0; i < 4; i++)
    {
        if (BlendWeight[i] == 0.f)
        {
            continue;
        }

        matrix  matBone = g_BoneMatrixArray[(int)BlendIndex[i]];

        Info += (mul(float4(Pos, 1.f), matBone) * BlendWeight[i]).xyz;
    }

    return Info;
}

VS_OUTPUT_SHADOW ShadowMapVS(float3 Pos : POSITION, float4 BlendWeight : BLENDWEIGHT, float4 BlendIndex : BLENDINDEX)
{
    VS_OUTPUT_SHADOW output = (VS_OUTPUT_SHADOW)0;

    float3  SkinningPos = Pos;

    if (g_MtrlAnimation3D == 1)
    {
        SkinningPos = Skinning(Pos, BlendWeight, BlendIndex);
    }

    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.ProjPos = mul(float4(SkinningPos, 1.f), g_matWVP);

    output.Pos = output.ProjPos;

    return output;
}

VS_OUTPUT_SHADOW ShadowMapStaticVS(float3 Pos : POSITION)
{
    VS_OUTPUT_SHADOW output = (VS_OUTPUT_SHADOW)0;

    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);

    output.Pos = output.ProjPos;

    return output;
}

float4 ShadowMapPS(VS_OUTPUT_SHADOW input) : SV_TARGET
{
    float4	Color = (float4)0.f;

    Color.rgb = (float3)(input.ProjPos.z / input.ProjPos.w);
    Color.w = input.ProjPos.w;

    return Color;
}

VS_OUTPUT_SHADOWINSTANCE ShadowMapInstancingVS(float3 Pos : POSITION, uint InstanceID : SV_InstanceID)
{
    VS_OUTPUT_SHADOWINSTANCE	output = (VS_OUTPUT_SHADOWINSTANCE)0;

    return output;
}

float4 ShadowMapInstancingPS(VS_OUTPUT_SHADOWINSTANCE input) : SV_TARGET
{
    float4	Color = (float4)0.f;

    return Color;
}
