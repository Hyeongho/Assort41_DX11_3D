#include "Share.fx"

struct VS_INPUT_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

cbuffer DropCBuffer : register(b9)
{
    float g_Drop;
    float3  g_DropEmpty;
}

VS_OUTPUT_UV DropVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output;

    output.Pos = mul(float4(input.Pos, 1.f), g_matWVP);

    output.UV = input.UV;

    return output;
}

float4 DropPS(VS_OUTPUT_UV input) : SV_TARGET
{
    float2 UV = input.UV + float2(0.f, g_Drop);
    input.UV.y += g_Drop;

    return g_BaseTexture.Sample(g_LinearSmp, UV);
}
