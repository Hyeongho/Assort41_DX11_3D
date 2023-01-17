#pragma once

#include "../EngineInfo.h"

class CCameraManager
{
	friend class CScene;

private:
	CCameraManager();
	~CCameraManager();

private:
	class CScene* m_Owner;

private:
	CSharedPtr<class CCameraComponent>	m_CurrentCamera;
	CSharedPtr<class CCameraComponent>	m_UICamera;

public:
	class CCameraComponent* GetCurrentCamera()	const
	{
		return m_CurrentCamera;
	}

	class CCameraComponent* GetUICamera()	const
	{
		return m_UICamera;
	}

public:
	void SetCurrentCamera(class CCameraComponent* Camera);
	void SetUICamera(class CCameraComponent* Camera);

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Save(FILE* File);
	void Load(FILE* File);
};

