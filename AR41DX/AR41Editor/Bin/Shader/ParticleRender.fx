#include "Share.fx"

struct VS_INPUT_PARTICLE
{
    float3 Pos : POSITION;
    uint InstanceID : SV_InstanceID;
};

struct VS_OUTPUT_PARTICLE
{
    uint InstanceID : TEXCOORD;
};

struct ParticleInfo
{
    uint Enable;
    float3 WorldPos;
    float3 Dir;
    float Speed;
    float LifeTime;
    float LifeTimeMax;
    float FallTime;
    float FallStartY;
};

struct ParticleInfoShare
{
    uint SpawnEnable;

    float3 ScaleMin;
    float3 ScaleMax;

    float4 ColorMin;
    float4 ColorMax;

    uint GravityEnable;
};

StructuredBuffer<ParticleInfo> g_ParticleArray : register(t30);
StructuredBuffer<ParticleInfoShare> g_ParticleShare : register(t31);

Texture2DMS<float4> g_GBufferDepth : register(t14);

VS_OUTPUT_PARTICLE ParticleVS(VS_INPUT_PARTICLE input)
{
    VS_OUTPUT_PARTICLE output = (VS_OUTPUT_PARTICLE)0;

    output.InstanceID = input.InstanceID;

    return output;
}

struct GS_OUTPUT_PARTICLE
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
    float4 ProjPos : POSITION;
};

static float3 g_ParticleLocalPos[4] =
{
    {-0.5f, 0.5f, 0.f},
    {0.5f, 0.5f, 0.f},
    {-0.5f, -0.5f, 0.f},
    {0.5f, -0.5f, 0.f},
};

[maxvertexcount(6)]
void ParticleGS(point VS_OUTPUT_PARTICLE input[1], inout TriangleStream<GS_OUTPUT_PARTICLE> output)
{
    uint InstanceID = input[0].InstanceID;

    if (g_ParticleArray[InstanceID].Enable == 0)
    {
        return;
    }

    GS_OUTPUT_PARTICLE  OutputArray[4] =
    {
        (GS_OUTPUT_PARTICLE)0.f,
        (GS_OUTPUT_PARTICLE)0.f,
        (GS_OUTPUT_PARTICLE)0.f,
        (GS_OUTPUT_PARTICLE)0.f
    };

    OutputArray[0].UV = float2(0.f, 0.f);
    OutputArray[1].UV = float2(1.f, 0.f);
    OutputArray[2].UV = float2(0.f, 1.f);
    OutputArray[3].UV = float2(1.f, 1.f);

    float Ratio = g_ParticleArray[InstanceID].LifeTime / g_ParticleArray[InstanceID].LifeTimeMax;

    float3 Scale = lerp(g_ParticleShare[0].ScaleMin, g_ParticleShare[0].ScaleMax, float3(Ratio, Ratio, Ratio));

    float4 Color = lerp(g_ParticleShare[0].ColorMin, g_ParticleShare[0].ColorMax, float4(Ratio, Ratio, Ratio, Ratio));

    for (int i = 0; i < 4; i++)
    {
        float3 ViewPos = mul(float4(g_ParticleArray[InstanceID].WorldPos, 1.f), g_matView);

        // 뷰 공간에서의 카메라의 축은 월드축과 동일하다.
        ViewPos = ViewPos + g_ParticleLocalPos[i] * Scale * 0.5f;

        OutputArray[i].ProjPos = mul(float4(ViewPos, 1.f), g_matProj);
        OutputArray[i].Pos = OutputArray[i].ProjPos;
        OutputArray[i].Color = Color;
    }

    output.Append(OutputArray[0]);
    output.Append(OutputArray[1]);
    output.Append(OutputArray[3]);
    output.RestartStrip();

    output.Append(OutputArray[0]);
    output.Append(OutputArray[3]);
    output.Append(OutputArray[2]);
    output.RestartStrip();
}

float4 ParticlePS(GS_OUTPUT_PARTICLE input) : SV_TARGET
{
    float4 Color = (float4)0;

    float4 TextureColor = g_BaseTexture.Sample(g_LinearSmp, input.UV);

    if (TextureColor.a == 0.f)
    {
        clip(-1);
    }

    float2 UV;
    UV.x = input.ProjPos.x / input.ProjPos.w * 0.5f + 0.5f;
    UV.y = input.ProjPos.y / input.ProjPos.w * -0.5f + 0.5f;

    int2 LoadPos = (int2)0;

    LoadPos.x = (int)(UV.x * g_Resolution.x);
    LoadPos.y = (int)(UV.y * g_Resolution.y);

    float4 GBufferDepth = g_GBufferDepth.Load(LoadPos, 0);

    float Alpha = 1.f;

    if (GBufferDepth.a > 0.f)
    {
        Alpha = (GBufferDepth.g - input.ProjPos.w) / 20.f;
    }

    Alpha = clamp(Alpha, 0.f, 1.f);


    Color.rgb = TextureColor.rgb * g_MtrlBaseColor.rgb;

    Color.a = TextureColor.a * g_MtrlOpacity * Alpha;

    return Color;
}
