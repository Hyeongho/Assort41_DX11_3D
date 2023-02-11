#include "ParticleRenderShader.h"

CParticleRenderShader::CParticleRenderShader()
{
}

CParticleRenderShader::~CParticleRenderShader()
{
}

bool CParticleRenderShader::Init()
{
	if (!LoadVertexShader("ParticleVS", TEXT("ParticleRender.fx"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadGeometryShader("ParticleGS", TEXT("ParticleRender.fx"), SHADER_PATH))
	{
		return false;
	}

	if (!LoadPixelShader("ParticlePS", TEXT("ParticleRender.fx"), SHADER_PATH))
	{
		return false;
	}

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
	{
		return false;
	}

	return true;
}

