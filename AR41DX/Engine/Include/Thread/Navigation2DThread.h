#pragma once
#include "NavigationThread.h"
class CNavigation2DThread :
    public CNavigationThread
{
	friend class CThreadManager;

protected:
	CNavigation2DThread();
	virtual ~CNavigation2DThread();

private:
	class CTileMapComponent* m_TileMap;
	class CNavigation* m_Navigation;

public:
	virtual void SetNavigationComponent(class CSceneComponent* NavComponent);

public:
	virtual void Run();
};

