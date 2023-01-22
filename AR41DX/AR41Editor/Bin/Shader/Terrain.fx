#include "Share.fx"

struct VS_INPUT_STATIC
{
    float3	Pos : POSITION;
    float3	Normal : NORMAL;
    float2	UV : TEXCOORD;
    float3	Tangent : TANGENT;
    float3	Binormal : BINORMAL;
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

    float2 DetaiUV = input.UV * g_TerrainDetailLevel;

    float4  TextureColor = g_BaseTexture.Sample(g_LinearSmp, input.UV);

    if (TextureColor.a == 0.f || g_MtrlOpacity == 0.f)
    {
        clip(-1);
    }

    output.GBuffer1.rgb = TextureColor.rgb;
    output.GBuffer1.a = TextureColor.a * g_MtrlOpacity;

    output.GBuffer2.rgb = ComputeBumpNormal(input.Tangent, input.Binormal, input.Normal, DetaiUV);
    output.GBuffer2.a = 1.f;

    // ���������� �̿��Ͽ� ��ȯ�� ���������� ��ġ���� w�� �����������
    // z���� �״�� ����.
    output.GBuffer3.r = input.ProjPos.z / input.ProjPos.w;
    output.GBuffer3.g = input.ProjPos.w;
    output.GBuffer3.b = g_MtrlSpecularColor.w;
    output.GBuffer3.a = 1.f;

    output.GBuffer4.r = ConvertColor(g_MtrlBaseColor);
    output.GBuffer4.g = ConvertColor(g_MtrlAmbientColor);

    float4  SpecularColor = g_MtrlSpecularColor;

    if (g_MtrlBaseColor.a == 1.f)
    {
        SpecularColor.rgb = g_SpecularTexture.Sample(g_LinearSmp, DetaiUV).rrr;
    }

    output.GBuffer4.b = ConvertColor(SpecularColor);

    float4  EmissiveColor = g_MtrlEmissiveColor;

    if (g_MtrlEmissiveColor.a == 1.f)
    {
        EmissiveColor.rgb = g_EmissiveTexture.Sample(g_LinearSmp, DetaiUV).rgb;
    }

    output.GBuffer4.a = ConvertColor(EmissiveColor);

    return output;
}
