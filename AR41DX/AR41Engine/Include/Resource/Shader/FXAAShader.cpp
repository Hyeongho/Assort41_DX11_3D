#include "FXAAShader.h"

CFXAAShader::CFXAAShader()
{
}

CFXAAShader::~CFXAAShader()
{
}

bool CFXAAShader::Init()
{
	if (!LoadVertexShader("main", TEXT("VSFXAA.fx"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadPixelShader("main", TEXT("PSFXAA.fx"), SHADER_PATH))
	{
		return false;
	}

	return true;
}
