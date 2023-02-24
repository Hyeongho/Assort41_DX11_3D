#include "LightCelShader.h"

CLightCelShader::CLightCelShader()
{
}

CLightCelShader::~CLightCelShader()
{
}

bool CLightCelShader::Init()
{
	if (!LoadVertexShader("LightAccVS", TEXT("Light.fx"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadPixelShader("CelShaderAccPS", TEXT("Light.fx"), SHADER_PATH))
	{
		return false;
	}

	return true;
}
