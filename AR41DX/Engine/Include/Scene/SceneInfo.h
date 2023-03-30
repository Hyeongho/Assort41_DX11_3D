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
	std::string	m_FileName;
	std::string	m_PrevFileName;
protected:
	void SetClassTypeName(const std::string& Name)
	{
		m_ClassTypeName = Name;
	}

public:
	void SetPlayerObject(class CGameObject* Player);
	CSharedPtr<class CGameObject> GetPlayerObject() const
	{
		return m_PlayerObject;
	}

	void LoadComplete();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSceneInfo* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	virtual void SceneChangeComplete();

	const std::string& GetFileName()	const
	{
		return m_FileName;
	}
	const std::string& GetPrevFileName()	const
	{
		return m_PrevFileName;
	}
	void SetFileName(const std::string& name)
	{
		m_FileName = name;
	}
	void SetPrevFileName(const std::string& name)
	{
		m_PrevFileName = name;
	}
};

