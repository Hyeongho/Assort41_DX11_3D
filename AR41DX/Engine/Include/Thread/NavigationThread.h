#pragma once

#include "Thread.h"
#include "ThreadQueue.h"

class CNavigationThread :
    public CThread
{
	friend class CThreadManager;

protected:
	CNavigationThread();
	virtual ~CNavigationThread();

protected:
	CThreadQueue m_InputQueue;

public:
	int GetWorkCount()
	{
		return m_InputQueue.size();
	}

public:
	virtual void SetNavigationComponent(class CSceneComponent* TileMap) = 0;

	void AddInputData(class CNavigationAgent* Agent, const Vector2& End);
	void AddInputData(class CNavigationAgent3D* Agent, const Vector3& End);

public:
	virtual void Run() = 0;
};

