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
	//class CTerrainComponent* m_Terrain;
	class CNavigationMesh* m_NavMesh;
	class CNavigation3D* m_Navigation;

public:
	class CNavigationMesh* GetNavigationMesh() const
	{
		return m_NavMesh;
	}

	class CNavigation3D* GetNavigation() const
	{
		return m_Navigation;
	}

public:
	void SetNavigationMesh(class CNavigationMesh* NavMesh);
	virtual void SetNavigationComponent(class CSceneComponent* NavComponent);

public:
	virtual void Run();
};

