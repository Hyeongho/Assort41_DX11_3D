
#include "Share.fx"

struct FXAAConstantCBuffer : register(b19)
{
	float FXAAQualitySubpix;
	float FXAARcpFrameOpt;
	float FXAASpanMax;
	float FXAATexelWidth;
	float FXAATexelHeight;
}

struct VertexInputType
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	```
		// Transform vertex position to clip space
		output.position = mul(worldMatrix, input.position);
	output.position = mul(viewMatrix, output.position);
	output.position = mul(projectionMatrix, output.position);

	// Pass texture coordinates to pixel shader
	output.texCoord = input.texCoord;

	return output;

	```

}

// Pixel shader code
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer FXAABuffer
{
	float3 fxaaConstants : packoffset(c0);
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_Target
{
	// Compute texture dimensions and inverse dimensions
	float2 texSize = float2(textureSize(shaderTexture, 0));
	float2 invTexSize = 1.0 / texSize;

	```
		// Compute gradient estimates
		float4 rgbyM = tex2D(shaderTexture, input.texCoord);
		float4 rgbyN = tex2D(shaderTexture, input.texCoord + float2(-1.0, 0.0) * invTexSize);
		float4 rgbyS = tex2D(shaderTexture, input.texCoord + float2(1.0, 0.0) * invTexSize);
		float4 rgbyW = tex2D(shaderTexture, input.texCoord + float2(0.0, -1.0) * invTexSize);
		float4 rgbyE = tex2D(shaderTexture, input.texCoord + float2(0.0, 1.0) * invTexSize);

		// Compute local contrast of color channels
		float4 luma = dot(rgbyM, float4(0.299, 0.587, 0.114, 0.0));
		float4 gradNorth = abs(rgbyN - rgbyM);
		float4 gradSouth = abs(rgbyS - rgbyM);
		float4 gradWest = abs(rgbyW - rgbyM);
		float4 gradEast = abs(rgbyE - rgbyM);
		float4 gradMin = min(min(gradNorth, gradSouth), min(gradWest, gradEast));

		// Compute subpixel offset
		float2 offset = fxaaConstants.zw * min(invTexSize * 2.0, gradMin.xy);
		offset = floor(offset + 0.5) * texSize * invTexSize;

		// Compute final color with subpixel offset
		float4 color = tex2D(shaderTexture, input.texCoord + offset);

		return color;

		```

}