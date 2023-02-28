#pragma once

#include "Scene.h"

class CSceneManager
{
private:
	CScene* m_Scene;
	CScene* m_NextScene;

public:
	CScene* GetScene()	const
	{
		return m_Scene;
	}


	CScene* GetNextScene()	const
	{
		return m_NextScene;
	}

public:
	bool Init();
	bool Update(float DeltaTime);
    bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);

private:
	bool ChangeScene();

public:
	void CreateNextScene(bool AutoChange = true);
	void ChangeNextScene();

public:
	template <typename T>
	bool CreateSceneInfo(bool Current = true)
	{
		if (Current)
			return m_Scene->CreateSceneInfo<T>();

		return m_NextScene->CreateSceneInfo<T>();
	}

	template <typename T>
	bool CreateSceneInfo(bool current, const std::string& name, const std::string& prevName = "")
	{
		if (current)
		{
			return m_Scene->CreateSceneInfo<T>(name, prevName);
		}
		m_Scene->FindObject("Gizmo")->Destroy();
		return m_NextScene->CreateSceneInfo<T>(name, prevName);
	}
	DECLARE_SINGLE(CSceneManager)
};

