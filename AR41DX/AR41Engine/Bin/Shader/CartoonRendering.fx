#include "Share.fx"

//Texture2D<float4> DiffuseTexture : register(t9);
Texture2D DiffuseTexture : register(t9);

SamplerState Linear : register(s1);

cbuffer CartoonCBuffer : register(b15) {
    matrix WorldViewProj;
    matrix World;
    matrix View;
    matrix InvView;
    float4 LightDir;
    float4 ObjectColor;
}

// Vertex input structure
struct VertexInput {
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

// Vertex output structure
struct VSOutput {
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float4 Color : COLOR;
};

// Vertex shader entry point
VSOutput VS_Main(VertexInput input) {
    VSOutput output;

    // Transform the vertex position to clip space
    output.Position = mul(float4(input.Position, 1.0f), WorldViewProj);

    // Compute the world-space normal
    float3 worldNormal = normalize(mul(input.Normal, World));

    // Compute the lighting intensity
    float NdotL = dot(worldNormal, -LightDir.xyz);

    // Clamp the lighting intensity to the range [0, 1]
    NdotL = saturate(NdotL);

    // Compute the cartoon shading color
    float3 toonColor = ObjectColor.rgb * (NdotL * 0.5f + 0.5f);

    // Compute the texture color
    float4 texColor = Texture.Sample(Sampler, input.TexCoord);

    // Combine the cartoon shading color and texture color
    output.Color = float4(toonColor * texColor.rgb, texColor.a);

    // Pass the texture coordinate to the pixel shader
    output.TexCoord = input.TexCoord;

    return output;
}

// Pixel shader entry point
float4 PS_Main(float4 color : COLOR, float2 texCoord : TEXCOORD) : SV_Target{
    // Apply the cartoon edge effect
    float edge = 1.0f - length(ddx(texCoord) + ddy(texCoord));
    edge = saturate(edge * 10.0f);

    // Combine the edge color and texture color
    return lerp(color, float4(0, 0, 0, color.a), edge);
}