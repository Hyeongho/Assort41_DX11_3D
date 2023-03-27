#include "CartoonShader.h"

CCartoonShader::CCartoonShader()
{
}

CCartoonShader::~CCartoonShader()
{
}

bool CCartoonShader::Init()
{
	if (!LoadVertexShader("mainVS", TEXT("CartoonRendering.fx"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadPixelShader("mainPS", TEXT("CartoonRendering.fx"), SHADER_PATH))
	{
		return false;
	}

	return true;
}
