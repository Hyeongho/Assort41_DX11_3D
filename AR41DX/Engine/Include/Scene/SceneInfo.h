#pragma once

#include "../EngineInfo.h"

class CSceneInfo
{
	friend class CScene;
	friend class CSceneManager;

protected:
	CSceneInfo();
	CSceneInfo(const CSceneInfo& Info);
	virtual ~CSceneInfo();

protected:
	class CScene* m_Owner;
	CSharedPtr<class CGameObject> m_PlayerObject;
	std::string	m_ClassTypeName;
	std::string	m_PlayerObjectName;

protected:
	void SetClassTypeName(const std::string& Name)
	{
		m_ClassTypeName = Name;
	}

public:
	void SetPlayerObject(class CGameObject* Player);
	void LoadComplete();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSceneInfo* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	virtual void SceneChangeComplete();
};

