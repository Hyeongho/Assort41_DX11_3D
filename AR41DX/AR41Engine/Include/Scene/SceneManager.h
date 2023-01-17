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

	DECLARE_SINGLE(CSceneManager)
};

