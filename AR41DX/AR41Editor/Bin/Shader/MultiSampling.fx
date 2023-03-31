
#include "Share.fx"

Texture2DMS<float4> g_InputMSTex : register(t8); 
Texture2DMS<float4> g_ScreenTex : register(t21);

// �� ���ۿ� ���ؽ� ����
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
    // VertexID : �� ���۸� ����� �� ���. �� ��° ������ Ȯ���ϴ� �뵵
    VS_OUTPUT_NULL output = (VS_OUTPUT_NULL)0;

    output.Pos = float4(g_NullPos[vertexID], 0.f, 1.f);
    output.UV = g_NullUV[vertexID];

    return output;
}

// ��Ƽ���ø� �ؽ�ó�� �Ϲ� ���ø� �ؽ�ó�� �ٲ��ִ� ���̴� �ڵ�.
float4 ConvertMSPS(in VS_OUTPUT_NULL input) : SV_Target
{
   int2 loadPos = (int2)(input.UV * g_GraphicInputSize);
   return g_InputMSTex.Load(loadPos, 0);
}
