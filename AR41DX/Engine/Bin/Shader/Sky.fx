#include "Share.fx"

struct VS_INPUT_SKY
{
	float3	Pos : POSITION;
};

struct VS_OUTPUT_SKY
{
	float4	Pos : SV_POSITION;
	float3	UV : TEXCOORD;
};

TextureCube g_SkyTexture : register(t10);

VS_OUTPUT_SKY SkyVS(VS_INPUT_SKY input)
{
	VS_OUTPUT_SKY	output = (VS_OUTPUT_SKY)0;

	output.Pos = mul(float4(input.Pos, 1.f), g_matWVP).xyww;
	output.UV = input.Pos;

	return output;
}

float4 SkyPS(VS_OUTPUT_SKY input) : SV_TARGET
{
	float4	Color = g_SkyTexture.Sample(g_LinearSmp, input.UV);

	return Color;
}
