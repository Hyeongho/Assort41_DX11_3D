#pragma once
#include "NavigationThread.h"
class CNavigation3DThread :
    public CNavigationThread
{
	friend class CThreadManager;

protected:
	CNavigation3DThread();
	virtual ~CNavigation3DThread();

private:
	class CTerrainComponent* m_Terrain;
	class CNavigation3D* m_Navigation;

public:
	virtual void SetNavigationComponent(class CSceneComponent* NavComponent);

public:
	virtual void Run();
};

