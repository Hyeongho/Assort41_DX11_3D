#include "ParticleManager.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
}

bool CParticleManager::Init()
{
	CreateParticle("Bubble");
	CParticle* particle = FindParticle("Bubble");
	particle->SetMaterial("Bubble");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleStartMin(Vector3(-100.f, -100.f, -100.f));
	particle->SetParticleStartMax(Vector3(100.f, 100.f, 100.f));
	particle->SetParticleSpawnCountMax(1000);
	particle->SetParticleScaleMin(Vector3(20.f, 20.f, 20.f));
	particle->SetParticleScaleMax(Vector3(50.f, 50.f, 50.f));
	particle->SetParticleLifeTimeMin(2.f);
	particle->SetParticleLifeTimeMax(3.f);
	particle->SetParticleColorMin(Vector4(1.f, 0.f, 0.f, 1.f));
	particle->SetParticleColorMax(Vector4(0.f, 1.f, 0.f, 1.f));
	particle->SetParticleSpeedMin(100.f);
	particle->SetParticleSpeedMax(200.f);
	particle->SetParticleMoveEnable(false);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDir(Vector3(0.f, 1.f, 0.f));
	particle->SetParticleMoveDirEnable(true);
	particle->SetParticleMoveAngle(Vector3(0.f, 0.f, 0.f));

	CreateParticle("LoadingBubble");
	particle = FindParticle("LoadingBubble");
	particle->SetMaterial("Bubble");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleStartMin(Vector3(-100.f, -100.f, -100.f));
	particle->SetParticleStartMax(Vector3(100.f, 100.f, 100.f));
	particle->SetParticleSpawnCountMax(1000);
	particle->SetParticleScaleMin(Vector3(10.f, 10.f, 10.f));
	particle->SetParticleScaleMax(Vector3(40.f, 40.f, 40.f));
	particle->SetParticleLifeTimeMin(2.f);
	particle->SetParticleLifeTimeMax(3.f);
	particle->SetParticleColorMin(Vector4(1.f, 0.f, 0.f, 1.f));
	particle->SetParticleColorMax(Vector4(0.f, 1.f, 0.f, 1.f));
	particle->SetParticleSpeedMin(100.f);
	particle->SetParticleSpeedMax(200.f);
	particle->SetParticleMoveEnable(true);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDir(Vector3(0.f, 1.f, 0.f));
	particle->SetParticleMoveDirEnable(true);
	particle->SetParticleMoveAngle(Vector3(0.f, 0.f, 0.f));
	return true;
}

bool CParticleManager::CreateParticle(const std::string& Name)
{
	CParticle* Particle = FindParticle(Name);

	if (Particle)
	{
		return false;
	}

	Particle = new CParticle;

	Particle->SetName(Name);

	if (!Particle->Init())
	{
		SAFE_RELEASE(Particle);
		return false;
	}

	m_mapParticle.insert(std::make_pair(Name, Particle));

	return true;
}

CParticle* CParticleManager::FindParticle(const std::string& Name)
{
	auto iter = m_mapParticle.find(Name);

	if (iter == m_mapParticle.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CParticleManager::ReleaseParticle(const std::string& Name)
{
	auto iter = m_mapParticle.find(Name);

	if (iter != m_mapParticle.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapParticle.erase(iter);
		}
	}
}