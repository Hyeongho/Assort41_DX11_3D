
#include "DeferredRenderShader.h"

CDeferredRenderShader::CDeferredRenderShader()
{
}

CDeferredRenderShader::~CDeferredRenderShader()
{
}

bool CDeferredRenderShader::Init()
{
	if (!LoadVertexShader("DeferredVS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("DeferredPS", TEXT("Light.fx"), SHADER_PATH))
		return false;

	return true;
}
