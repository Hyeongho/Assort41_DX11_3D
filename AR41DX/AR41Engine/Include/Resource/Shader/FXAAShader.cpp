#include "FXAAShader.h"

CFXAAShader::CFXAAShader()
{
}

CFXAAShader::~CFXAAShader()
{
}

bool CFXAAShader::Init()
{
	if (!LoadVertexShader("NullVS", TEXT("FXAA.hlsl"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadPixelShader("FXAAPS", TEXT("FXAA.hlsl"), SHADER_PATH))
	{
		return false;
	}

	return true;
}
