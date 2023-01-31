#pragma once

#include "../EngineInfo.h"

class CNavigationManager
{
	friend class CScene;
	friend class CSceneManager;

protected:
	CNavigationManager();
	virtual ~CNavigationManager();

protected:
	class CScene* m_Owner;
	std::vector<class CNavigationThread*>	m_vecNavThread;

public:
	void AddNavigationThread(class CNavigationThread* Thread)
	{
		m_vecNavThread.push_back(Thread);
	}

public:
	virtual void Start() = 0;
	virtual bool Init() = 0;
	virtual void Move(class CNavigationAgent* Agent, const Vector2& End);
	virtual void Move(class CNavigationAgent* Agent, const Vector3& End);
};

