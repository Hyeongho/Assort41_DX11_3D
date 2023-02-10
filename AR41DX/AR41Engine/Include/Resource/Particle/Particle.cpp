#include "Particle.h"
#include "../Shader/StructuredBuffer.h"

CParticle::CParticle() : m_Scene(nullptr)
{
}

CParticle::~CParticle()
{
}

bool CParticle::Init()
{
	return true;
}
