#pragma once

#include "../EngineInfo.h"

class CLightManager
{
	friend class CScene;

private:
	CLightManager();
	~CLightManager();

private:
	class CScene* m_Owner;

private:
	std::list<CSharedPtr<class CLightComponent>>	m_LightList;
	CSharedPtr<class CGameObject>	m_GlobalLightObj;
	CSharedPtr<class CLightComponent>	m_GlobalLightComponent;

	class CStructuredBuffer* m_LightInfoBuffer;
	size_t				m_LightInfoBufferCount;

	CSharedPtr<class CShader>	m_LightAccShader;

public:
	class CGameObject* GetGlobalLightObj()	const
	{
		return m_GlobalLightObj;
	}

	class CLightComponent* GetGlobalLightComponent()	const
	{
		return m_GlobalLightComponent;
	}

public:
	void SetGlobalLightObject(class CGameObject* light);
	void AddLight(class CLightComponent* Light);
	void DeleteLight(class CLightComponent* Light);
	void DeleteLight(const std::string& Name);

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void Render();
	void Save(FILE* File);
	void Load(FILE* File);
};

