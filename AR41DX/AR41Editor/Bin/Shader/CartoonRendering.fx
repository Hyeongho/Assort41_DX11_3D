#include "Share.fx"

SamplerState PointClamp : register(s4); // ¾Æ¿ô¶óÀÎ¿ë Point/Clamp »ùÇÃ·¯

cbuffer CartoonCBuffer : register(b2)
{
    float4x4 worldViewProj;
    float OutlineThickness;
    float4 OutlineColor;
    float3 Empty;
}

struct VS_INPUT
{
    float3 pos : POSITION;
};

struct PS_INPUT
{
    float4 worldPos : SV_POSITION;
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.worldPos = mul(float4(input.pos, 1.0f), worldViewProj);
    return output;
}

float4 mainPS(PS_INPUT input) : SV_Target
{
    float4 color = g_CartoonTexture.Sample(PointClamp, input.worldPos.xy);
    float4 outline = float4(0, 0, 0, 0);
    for (int i = 0; i < 8; i++)
    {
        float2 offset = float2(cos(i * 0.25f * 3.14159265f), sin(i * 0.25f * 3.14159265f));
        float4 sample = g_CartoonTexture.Sample(PointClamp, input.worldPos.xy + OutlineThickness * offset);
        outline += saturate(sample - color);
    }
    outline.a = 1;
    return lerp(color, OutlineColor, outline);
}