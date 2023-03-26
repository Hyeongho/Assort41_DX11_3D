//#include "Share.fx"
//
////Texture2D<float4> DiffuseTexture : register(t9);
////Texture2D g_InputTexture  : register(t9);
////SamplerState g_Sampler  : register(s1); // Linear
//
//cbuffer CartoonCBuffer : register(b15)
//{
//    matrix WorldViewProj;
//    matrix World;
//    matrix View;
//    matrix InvView;
//    float3 LightDir;
//    float4 ObjectColor;
//}
//
//// Vertex shader entry point
//VS_OUTPUT main(float4 position : POSITION, float3 normal : NORMAL, float2 texcoord : TEXCOORD0)
//{
//    VS_OUTPUT output;
//
//    // Transform the vertex position and normal
//    float4 worldPosition = mul(position, World);
//    float4 viewPosition = mul(worldPosition, View);
//    float4 clipPosition = mul(viewPosition, WorldViewProj);
//
//    output.position = clipPosition;
//
//    // Compute the view-space normal and light direction
//    float3 viewNormal = normalize(mul(normal, World));
//    float3 viewLightDir = normalize(mul(LightDir, InvView));
//
//    // Compute the diffuse lighting term
//    float diffuse = saturate(dot(viewNormal, -viewLightDir));
//
//    // Apply the cartoon shading effect
//    if (diffuse > 0.8)
//    {
//        output.color = ObjectColor;
//    }
//    else if (diffuse > 0.2)
//    {
//        output.color = lerp(ObjectColor, float4(0.0, 0.0, 0.0, 1.0), 0.5);
//    }
//    else
//    {
//        output.color = float4(0.0, 0.0, 0.0, 1.0);
//    }
//
//    // Pass the texture coordinate to the pixel shader
//    output.texcoord = texcoord;
//
//    return output;
//}
//
//// Pixel shader entry point
//float4 main(VS_OUTPUT input) : SV_TARGET
//{
//    // Sample the texture using the provided texture coordinate
//    float4 texColor = tex.Sample(DefaultSampler, input.texcoord);
//
//    // Combine the texture color with the cartoon shading
//    return input.color * texColor;
//}