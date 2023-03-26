#include "CartoonShader.h"

CCartoonShader::CCartoonShader()
{
}

CCartoonShader::~CCartoonShader()
{
}

bool CCartoonShader::Init()
{
	if (!LoadVertexShader("main", TEXT("CartoonRendering.fx"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadPixelShader("main", TEXT("CartoonRendering.fx"), SHADER_PATH))
	{
		return false;
	}

	return true;
}
