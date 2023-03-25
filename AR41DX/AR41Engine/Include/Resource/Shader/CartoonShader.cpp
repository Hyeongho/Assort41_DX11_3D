#include "CartoonShader.h"

CCartoonShader::CCartoonShader()
{
}

CCartoonShader::~CCartoonShader()
{
}

bool CCartoonShader::Init()
{
	if (!LoadVertexShader("VS_Main", TEXT("CartoonRendering.fx"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadPixelShader("PS_Main", TEXT("CartoonRendering.fx"), SHADER_PATH))
	{
		return false;
	}

	return true;
}
