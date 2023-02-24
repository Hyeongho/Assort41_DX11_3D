
#include "Share.fx"

Texture2DMS<float4> g_GBuffer1Tex : register(t14);
Texture2DMS<float4> g_GBuffer2Tex : register(t15);
Texture2DMS<float4> g_GBuffer3Tex : register(t16);
Texture2DMS<float4> g_GBuffer4Tex : register(t17);
Texture2DMS<float4> g_GBuffer5Tex : register(t18);
Texture2DMS<float4> g_GBuffer6Tex : register(t19);

struct VS_OUTPUT_DECAL
{
	float4	Pos : SV_POSITION;
	float4	ProjPos : POSITION;
};

struct PS_OUTPUT_DECAL
{
	float4	Color : SV_TARGET;
	float4	Normal : SV_TARGET1;
	float4	Material : SV_TARGET2;
};

float4 DecalDebugVS(float3 Pos : POSITION) : SV_POSITION
{
	return mul(float4(Pos, 1.f), g_matWVP);
}

float4 DecalDebugPS() : SV_TARGET
{
	return g_MtrlBaseColor;
}

VS_OUTPUT_DECAL DecalVS(float3 Pos : POSITION)
{
	VS_OUTPUT_DECAL	output = (VS_OUTPUT_DECAL)0;

	output.ProjPos = mul(float4(Pos, 1.f), g_matWVP);
	output.Pos = output.ProjPos;

	return output;
}

PS_OUTPUT_DECAL DecalPS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL	output = (PS_OUTPUT_DECAL)0;

	float2  UV;
	UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
	UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

	int2 LoadPos = (int2)0;

	LoadPos.x = (int)(UV.x * g_Resolution.x);
	LoadPos.y = (int)(UV.y * g_Resolution.y);

	float4 GBuffer2Color = g_GBuffer2Tex.Load(LoadPos, 0);

	float4 GBuffer3Color = g_GBuffer3Tex.Load(LoadPos, 0);

	// ReceiveDecal이 false인 픽셀은 제외한다.
	if (GBuffer3Color.a == 0.f)
	{
		clip(-1);
	}

	// 조명을 그려내는 투영공간의 위치를 이용해서 물체의 위치를 구해내야 한다.
	float4  ProjPos;

	// 여기에 구성한 값은 출력되어 있는 물체의 w로 나누어준 값이 저장된것이다.
	ProjPos.x = (UV.x * 2.f - 1.f) * GBuffer3Color.g;
	ProjPos.y = (UV.y * -2.f + 1.f) * GBuffer3Color.g;
	ProjPos.z = GBuffer3Color.r * GBuffer3Color.g;
	ProjPos.w = GBuffer3Color.g;

	// 조명연산을 위한 Normal은 뷰공간에 있으므로 뷰 공간의 위치를
	// 구해준다.
	float3 LocalPos = mul(ProjPos, g_matInvWVP).xyz;

	clip(0.5f - abs(LocalPos));

	// -0.5 ~ 0.5 사이의 값을 이용하여 UV 값으로 만들어내야 한다.
	UV = LocalPos.xz + float2(0.5f, 0.5f);
	UV.y = 1.f - UV.y;

	float4  TextureColor = g_BaseTexture.Sample(g_LinearSmp, UV);

	if (TextureColor.a == 0.f)
	{
		clip(-1);
	}

	float4 GBuffer5Color = g_GBuffer5Tex.Load(LoadPos, 0);
	float4 GBuffer6Color = g_GBuffer6Tex.Load(LoadPos, 0);

	float3	Tangent = GBuffer5Color.rgb;
	float3	Normal = GBuffer6Color.rgb;
	float3	Binormal = cross(Normal, Tangent);

	output.Normal.rgb = ComputeBumpNormal(Tangent, Binormal, Normal, UV);
	output.Normal.a = 1.f;

	output.Color.rgb = TextureColor.rgb;
	output.Color.a = TextureColor.a * g_MtrlOpacity;

	output.Material.r = ConvertColor(g_MtrlBaseColor);
	output.Material.g = ConvertColor(g_MtrlAmbientColor);

	float4  SpecularColor = g_MtrlSpecularColor;

	if (g_MtrlBaseColor.a == 1.f)
	{
		SpecularColor.rgb = g_SpecularTexture.Sample(g_LinearSmp, UV).rgb;
	}

	output.Material.b = ConvertColor(SpecularColor);

	float4  EmissiveColor = g_MtrlEmissiveColor;

	if (g_MtrlEmissiveColor.a == 1.f)
	{
		EmissiveColor.rgb = g_EmissiveTexture.Sample(g_LinearSmp, UV).rgb;
	}

	output.Material.a = ConvertColor(EmissiveColor);

	return output;
}

