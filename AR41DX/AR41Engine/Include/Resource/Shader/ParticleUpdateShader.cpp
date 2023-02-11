#include "ParticleUpdateShader.h"

CParticleUpdateShader::CParticleUpdateShader()
{
	SetTypeID<CParticleUpdateShader>();
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}

bool CParticleUpdateShader::Init()
{
	if (!LoadComputeShader("ParticleUpdateCS", TEXT("ParticleUpdate.fx"), SHADER_PATH))
	{
		return false;
	}

	return true;
}
