
#include "Share.fx"

cbuffer ColliderCBuffer : register(b10)
{
    float4 g_ColliderColor;
    matrix g_ColliderWVP;
};

float4 ColliderVS(float3 Pos : POSITION) : SV_Position
{
    return mul(float4(Pos, 1.f), g_ColliderWVP);
}

float4 ColliderPS() : SV_TARGET
{
    return g_ColliderColor;
}

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


// 인자로 VS_INPUT_COLOR를 쓰면 저 구조체에 지정된 입력 레지스터에서 값을
// 가져와서 사용하므로 인자를 따로 넣어줄 필요가 없다.
VS_OUTPUT_UV ColliderImageVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;
    
    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.Pos = mul(float4(input.Pos, 1.f), g_ColliderWVP);
    output.UV = input.UV;
    
    return output;
}

float4 ColliderImagePS(VS_OUTPUT_UV input) : SV_TARGET
{
    return g_BaseTexture.Sample(g_PointSmp, input.UV);
}
