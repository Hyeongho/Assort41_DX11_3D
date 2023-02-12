#include "Share.fx"

struct VS_INPUT_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 Pos : SV_POSITION;
    float4 ProjPos : POSITION;
    float2 UV : TEXCOORD;
};

Texture2DMS<float4> g_GBufferDepth : register(t14);

VS_OUTPUT_UV BillboardVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

    // mul : 행렬 곱. g_matWVP 는 World * View * Proj 이므로 정점을 여기에 곱하게 되면
    // 투영 공간으로 변환된 정점의 위치가 나온다.
    output.ProjPos = mul(float4(input.Pos, 1.f), g_matWVP);
    output.Pos = output.ProjPos;
    //output.UV = UpdateAnimation2D(input.UV);
    output.UV = input.UV;

    return output;
}

PS_OUTPUT_SINGLE BillboardPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    if (g_MtrlOpacity == 0.f)
    {
        clip(-1);
    }

    float4  TextureColor = g_BaseTexture.Sample(g_LinearSmp, input.UV);

    if (TextureColor.a == 0.f)
    {
        clip(-1);
    }

    float2  UV;
    UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
    UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

    int2 LoadPos = (int2)0;

    LoadPos.x = (int)(UV.x * g_Resolution.x);
    LoadPos.y = (int)(UV.y * g_Resolution.y);

    float4 GBufferDepth = g_GBufferDepth.Load(LoadPos, 0);

    float   Alpha = 1.f;

    if (GBufferDepth.a > 0.f)
    {
        Alpha = (GBufferDepth.g - input.ProjPos.w) / 20.f;
    }

    Alpha = clamp(Alpha, 0.f, 1.f);

    output.Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb;

    output.Color.a = TextureColor.a * g_MtrlOpacity * Alpha;

    return output;
}
