
#include "Share.fx"

struct VS_INPUT_STATIC
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
};

struct VS_OUTPUT_STATIC
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 ProjPos : POSITION;
    float3 ViewPos : POSITION1;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
};

cbuffer TerrainCBuffer : register(b10)
{
    float g_TerrainDetailLevel;
    int g_TerrainSplatCount;
    float2 g_TerrainEmpty;
};

Texture2DArray g_SplatDifTextureArray: register(t10);
Texture2DArray g_SplatNormalTextureArray: register(t11);
Texture2DArray g_SplatSpcTextureArray: register(t12);
Texture2DArray g_SplatAlphaTextureArray: register(t13);

float3 ComputeBumpNormalTerrain(float3 Tangent, float3 Binormal, float3 Normal, float2 UV, float2 OriginUV)
{
    float3  Result = Normal;

    if (g_MtrlAmbientColor.w == 1.f)
    {
        float4 NormalColor = g_NormalTexture.Sample(g_LinearSmp, UV);

        // Alpha Texture�� ������ ���´�.
        for (int i = 0; i < g_TerrainSplatCount; i++)
        {
            float4 Alpha = g_SplatAlphaTextureArray.Sample(g_LinearSmp, float3(OriginUV, i));

            float4 SplatNrm = g_SplatNormalTextureArray.Sample(g_LinearSmp, float3(UV, i));

            NormalColor.rgb = NormalColor.rgb * (1.f - Alpha.r) + SplatNrm.rgb * Alpha.r;
        }

        // ������ 0 ~ 1 ���̷� �����Ƿ� -1 ~ 1 ���̷� ��ȯ�Ѵ�.
        float3  ConvertNormal = NormalColor.rgb * 2.f - 1.f;

        // z�� ������ + �������� ������ش�.;
        //ConvertNormal.z = 1.f;
        ConvertNormal = normalize(ConvertNormal);

        // Tangent(X��), Binormal(Y��), Normal(Z��)�� �̿��ؼ� �������͸�
        // ��������� ��ȯ���ش�.
        float3x3 matConvert =
        {
            Tangent,
            Binormal,
            Normal
        };

        Result = normalize(mul(ConvertNormal, matConvert));
    }

    return Result;
}

VS_OUTPUT_STATIC TerrainVS(VS_INPUT_STATIC input)
{
    VS_OUTPUT_STATIC output = (VS_OUTPUT_STATIC)0;

    // mul : ��� ��. g_matWVP �� World * View * Proj �̹Ƿ� ������ ���⿡ ���ϰ� �Ǹ�
    // ���� �������� ��ȯ�� ������ ��ġ�� ���´�.
    output.ProjPos = mul(float4(input.Pos, 1.f), g_matWVP);

    output.ViewPos = mul(float4(input.Pos, 1.f), g_matWV).xyz;

    output.Normal = normalize(mul(float4(input.Normal, 0.f), g_matWV).xyz);
    output.Tangent = normalize(mul(float4(input.Tangent, 0.f), g_matWV).xyz);
    output.Binormal = normalize(mul(float4(input.Binormal, 0.f), g_matWV).xyz);

    output.Pos = output.ProjPos;
    output.UV = input.UV;

    return output;
}

PS_OUTPUT_GBUFFER TerrainPS(VS_OUTPUT_STATIC input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER)0;

    float2  DetailUV = input.UV * g_TerrainDetailLevel;

    float4  TextureColor = g_BaseTexture.Sample(g_LinearSmp, DetailUV);

    if (TextureColor.a == 0.f || g_MtrlOpacity == 0.f)
    {
        clip(-1);
    }

    float4  SpecularColor = g_MtrlSpecularColor;

    if (g_MtrlBaseColor.a == 1.f)
    {
        SpecularColor.rgb = g_SpecularTexture.Sample(g_LinearSmp, DetailUV).rrr;
    }

    // Alpha Texture�� ������ ���´�.
    for (int i = 0; i < g_TerrainSplatCount; ++i)
    {
        float4 Alpha = g_SplatAlphaTextureArray.Sample(g_LinearSmp, float3(input.UV, i));

        float4 SplatDif = g_SplatDifTextureArray.Sample(g_LinearSmp, float3(DetailUV, i));

        TextureColor.rgb = TextureColor.rgb * (1.f - Alpha.r) + SplatDif.rgb * Alpha.r;

        float4 SplatSpc = g_SplatSpcTextureArray.Sample(g_LinearSmp, float3(DetailUV, i));

        SpecularColor.rgb = SpecularColor.rgb * (1.f - Alpha.r) + SplatSpc.rgb * Alpha.r;
    }

    output.GBuffer1.rgb = TextureColor.rgb;
    output.GBuffer1.a = TextureColor.a * g_MtrlOpacity;

    output.GBuffer2.rgb = ComputeBumpNormalTerrain(input.Tangent, input.Binormal, input.Normal, DetailUV, input.UV);
    output.GBuffer2.a = 1.f;

    // ���������� �̿��Ͽ� ��ȯ�� ���������� ��ġ���� w�� �����������
    // z���� �״�� ����.
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

    output.GBuffer4.b = ConvertColor(SpecularColor);

    float4  EmissiveColor = g_MtrlEmissiveColor;

    if (g_MtrlEmissiveColor.a == 1.f)
    {
        EmissiveColor.rgb = g_EmissiveTexture.Sample(g_LinearSmp, DetailUV).rgb;
    }

    output.GBuffer4.a = ConvertColor(EmissiveColor);

    return output;
}
