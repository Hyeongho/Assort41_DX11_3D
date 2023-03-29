
#include "Share.fx"

struct VS_INPUT_DEBUG
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_DEBUG
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

Texture2D<float4> g_TargetTex : register(t10);
//Texture2DMS<float4> g_TargetMSTex : register(t10);


// 인자로 VS_INPUT_COLOR를 쓰면 저 구조체에 지정된 입력 레지스터에서 값을
// 가져와서 사용하므로 인자를 따로 넣어줄 필요가 없다.
VS_OUTPUT_DEBUG DebugVS(VS_INPUT_DEBUG input)
{
    VS_OUTPUT_DEBUG output = (VS_OUTPUT_DEBUG)0;

    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.Pos = mul(float4(input.Pos, 1.f), g_matWVP);
    output.UV = input.UV;

    return output;
}

PS_OUTPUT_SINGLE DebugPS(VS_OUTPUT_DEBUG input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    int2 Pos = (int2)0;

    Pos.x = (int)(input.UV.x * g_ShadowMapResolution.x);
    Pos.y = (int)(input.UV.y * g_ShadowMapResolution.y);

    float4 Color = g_TargetTex.Sample(g_LinearSmp, Pos / g_Resolution);

    if (Color.a == 0.f)
        clip(-1);

    output.Color = Color;

    return output;
}
