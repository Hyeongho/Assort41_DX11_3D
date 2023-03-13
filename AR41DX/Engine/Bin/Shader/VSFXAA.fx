#include "Share.fx"

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUT main(uint id : SV_VertexID)
{
    VS_OUT vso;
    vso.uv = float2((id << 1) & 2, id & 2);
    vso.pos = float4(vso.uv * float2(2.0f, -2.0f) +
        float2(-1.0f, 1.0f), 0.0f, 1.0f);
    return vso;
}