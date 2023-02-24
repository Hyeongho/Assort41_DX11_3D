
#include "Share.fx"

cbuffer ColliderCBuffer : register(b13)
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

struct VS_OUTPUT_COLLIDER3D
{
    float4  Pos : SV_POSITION;
    float4  ProjPos : POSITION;
    float3  ViewNormal : TEXCOORD;
};

VS_OUTPUT_COLLIDER3D Collider3DVS(float3 Pos : POSITION)
{
    VS_OUTPUT_COLLIDER3D    output = (VS_OUTPUT_COLLIDER3D)0;

    output.ProjPos = mul(float4(Pos, 1.f), g_ColliderWVP);
    output.Pos = output.ProjPos;
    output.ViewNormal = normalize(Pos);
    //output.ViewNormal = mul(float4(output.ViewNormal, 0.f), g_ColliderWV);

    return output;
}

PS_OUTPUT_GBUFFER Collider3DPS(VS_OUTPUT_COLLIDER3D input)
{
    PS_OUTPUT_GBUFFER   output = (PS_OUTPUT_GBUFFER)0;

    output.GBuffer1 = g_ColliderColor;

    output.GBuffer2.rgb = input.ViewNormal;
    output.GBuffer2.a = 1.f;

    output.GBuffer3.r = input.ProjPos.z / input.ProjPos.w;
    output.GBuffer3.g = input.ProjPos.w;
    output.GBuffer3.b = 1.f;
    output.GBuffer3.a = 0.f;

    output.GBuffer4.r = ConvertColor(float4(1.f, 1.f, 1.f, 1.f));
    output.GBuffer4.g = ConvertColor(float4(1.f, 1.f, 1.f, 1.f));
    output.GBuffer4.b = ConvertColor(float4(0.f, 0.f, 0.f, 1.f));
    output.GBuffer4.a = ConvertColor(float4(0.f, 0.f, 0.f, 1.f));

    return output;
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


// ���ڷ� VS_INPUT_COLOR�� ���� �� ����ü�� ������ �Է� �������Ϳ��� ����
// �����ͼ� ����ϹǷ� ���ڸ� ���� �־��� �ʿ䰡 ����.
VS_OUTPUT_UV ColliderImageVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

    // mul : ��� ��. g_matWVP �� World * View * Proj �̹Ƿ� ������ ���⿡ ���ϰ� �Ǹ�
    // ���� �������� ��ȯ�� ������ ��ġ�� ���´�.
    output.Pos = mul(float4(input.Pos, 1.f), g_ColliderWVP);
    output.UV = input.UV;

    return output;
}

float4 ColliderImagePS(VS_OUTPUT_UV input) : SV_TARGET
{
    return g_BaseTexture.Sample(g_PointSmp, input.UV);
}
