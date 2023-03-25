
#include "Share.fx"

Texture2DMS<float4> g_InputMSTex : register(t8); 
Texture2DMS<float4> g_ScreenTex : register(t21);

// 널 버퍼용 버텍스 정보
//static const float2 g_NullPos[4] = {
//   float2(-1.f, 1.f),
//   float2(1.f, 1.f),
//   float2(-1.f, -1.f),
//   float2(1.f, -1.f)
//};
//
//static const float2 g_NullUV[4] = {
//   float2(0.f, 0.f),
//   float2(1.f, 0.f),
//   float2(0.f, 1.f),
//   float2(1.f, 1.f)
//};

cbuffer ShadowCBuffer : register(b15)
{
    uint2 g_GraphicInputSize;
   // uint sampleCount;
};

struct VS_OUTPUT_NULL
{
    float4 Pos : SV_POSITION;
    float2 UV  : TEXCOORD;
};

VS_OUTPUT_NULL NullVS(uint vertexID : SV_VertexID)
{
    // VertexID : 널 버퍼를 출력할 때 사용. 몇 번째 점인지 확인하는 용도
    VS_OUTPUT_NULL output = (VS_OUTPUT_NULL)0;

    output.Pos = float4(g_NullPos[vertexID], 0.f, 1.f);
    output.UV = g_NullUV[vertexID];

    return output;
}

// 멀티샘플링 텍스처를 일반 샘플링 텍스처로 바꿔주는 쉐이더 코드.
float4 ConvertMSPS(in VS_OUTPUT_NULL input) : SV_Target
{
   int2 loadPos = (int2)(input.UV * g_GraphicInputSize);
   return g_InputMSTex.Load(loadPos, 0);
}
