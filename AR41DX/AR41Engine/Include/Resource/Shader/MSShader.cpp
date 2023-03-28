#include "MSShader.h"

CMSShader::CMSShader()
{
}

CMSShader::~CMSShader()
{
}

bool CMSShader::Init()
{
	if (!LoadVertexShader("NullVS", TEXT("MultiSampling.fx"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadPixelShader("ConvertMSPS", TEXT("MultiSampling.fx"), SHADER_PATH))
	{
		return false;
	}

	return true;
}
