#pragma once

#include "Particle.h"

class CParticleManager
{
	friend class CResourceManager;

private:
	CParticleManager();
	~CParticleManager();

private:
	std::unordered_map<std::string, CSharedPtr<CParticle>> m_mapParticle;

public:
	void GetParticleNames(std::vector<std::string>& vecNames)
	{
		auto	iter = m_mapParticle.begin();
		auto	iterEnd = m_mapParticle.end();
		for (; iter != iterEnd; ++iter)
		{
			vecNames.push_back(iter->first);
		}
	}
	bool Init();
	bool CreateParticle(const std::string& Name);
	CParticle* FindParticle(const std::string& Name);
	void ReleaseParticle(const std::string& Name);
};

