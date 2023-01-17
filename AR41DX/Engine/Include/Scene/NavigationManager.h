#pragma once

#include "../EngineInfo.h"

class CNavigationManager
{
	friend class CScene;
	friend class CSceneManager;

protected:
	CNavigationManager();
	~CNavigationManager();

private:
	class CScene* m_Owner;
	std::vector<class CNavigationThread*>	m_vecNavThread;

public:
	void AddNavigationThread(class CNavigationThread* Thread)
	{
		m_vecNavThread.push_back(Thread);
	}

public:
	void Start();
	bool Init();
	void Move(class CNavigationAgent* Agent, const Vector2& End);
};

