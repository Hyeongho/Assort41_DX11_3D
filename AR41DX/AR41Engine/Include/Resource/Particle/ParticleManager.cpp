#include "ParticleManager.h"

CParticleManager::CParticleManager()
{
}

CParticleManager::~CParticleManager()
{
}

bool CParticleManager::Init()
{
	CreateParticle("LoadingBubble");
	CParticle* particle = FindParticle("LoadingBubble");
	particle->SetMaterial("Bubble");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleStartMin(Vector3(-100.f, -50.f, -100.f));
	particle->SetParticleStartMax(Vector3(0.f, -30.f, 100.f));
	particle->SetParticleSpawnCountMax(1000);
	particle->SetParticleScaleMin(Vector3(10.f, 10.f, 10.f));
	particle->SetParticleScaleMax(Vector3(40.f, 40.f, 40.f));
	particle->SetParticleLifeTimeMin(2.f);
	particle->SetParticleLifeTimeMax(3.f);
	particle->SetParticleColorMin(Vector4(1.f,1.f, 1.f, 1.f));
	particle->SetParticleColorMax(Vector4(1.f, 1.f, 1.f, 1.f));
	particle->SetParticleSpeedMin(100.f);
	particle->SetParticleSpeedMax(200.f);
	particle->SetParticleMoveEnable(true);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDir(Vector3(1.f, 1.f, 0.f));
	particle->SetParticleMoveDirEnable(true);
	particle->SetParticleMoveAngle(Vector3(0.f, 45.f, 0.f));

	CreateParticle("SpongebobAtk");
	particle = FindParticle("SpongebobAtk");
	particle->SetMaterial("Bubble");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleSpawnCountMax(1000);
	particle->SetParticleScaleMin(Vector3(10.f, 10.f, 10.f));
	particle->SetParticleScaleMax(Vector3(40.f, 40.f, 40.f));
	particle->SetParticleLifeTimeMin(0.5f);
	particle->SetParticleLifeTimeMax(1.f);
	particle->SetParticleSpeedMin(100.f);
	particle->SetParticleSpeedMax(200.f);
	particle->SetParticleMoveEnable(true);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDirEnable(true);
	particle->SetParticleMoveDir(Vector3(1.f, 1.f, -1.f));

	CreateParticle("BashBubble");
	particle = FindParticle("BashBubble");
	particle->SetMaterial("Bubble");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleStartMin(Vector3(-100.f, 0.f, -100.f));
	particle->SetParticleStartMax(Vector3(100.f, 0.f, 100.f));
	particle->SetParticleSpawnCountMax(1000);
	particle->SetParticleScaleMin(Vector3(30.f, 30.f, 30.f));
	particle->SetParticleScaleMax(Vector3(60.f, 60.f, 60.f));
	particle->SetParticleLifeTimeMin(0.5f);
	particle->SetParticleLifeTimeMax(1.f);
	particle->SetParticleSpeedMin(100.f);
	particle->SetParticleSpeedMax(200.f);
	particle->SetParticleMoveEnable(true);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDirEnable(true);
	particle->SetParticleMoveAngle(Vector3(45.f, 45.f, 45.f));

	CreateParticle("PatrickAtk");
	particle = FindParticle("PatrickAtk");
	particle->SetMaterial("ShockWave");
	particle->SetParticleSpawnTime(0.01f);
	particle->SetParticleStartMin(Vector3(100.f, 50.f, 100.f));
	particle->SetParticleStartMax(Vector3(100.f, 50.f, 100.f));
	particle->SetParticleSpawnCountMax(1);
	particle->SetParticleScaleMin(Vector3(100.f, 20.f, 100.f));
	particle->SetParticleScaleMax(Vector3(100.f, 20.f, 100.f));
	particle->SetParticleLifeTimeMin(1.f);
	particle->SetParticleLifeTimeMax(1.f);
	particle->SetParticleMoveEnable(false);
	particle->SetParticleGravityEnable(false);
	particle->SetParticleMoveDirEnable(false);
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