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

private:
	class CTileMapComponent* m_TileMap;
	class CNavigation* m_Navigation;
	CThreadQueue	m_InputQueue;

public:
	int GetWorkCount()
	{
		return m_InputQueue.size();
	}

public:
	void SetTileMapComponent(class CTileMapComponent* TileMap);

	void AddInputData(class CNavigationAgent* Agent, const Vector2& End);

public:
	virtual void Run();
};

