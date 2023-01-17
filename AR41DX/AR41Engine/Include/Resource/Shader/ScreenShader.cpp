
#include "ScreenShader.h"

CScreenShader::CScreenShader()
{
}

CScreenShader::~CScreenShader()
{
}

bool CScreenShader::Init()
{
	if (!LoadVertexShader("ScreenVS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("ScreenPS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	return true;
}
