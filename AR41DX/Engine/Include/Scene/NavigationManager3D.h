#pragma once

#include "NavigationManager.h"

class CNavigationManager3D :
    public CNavigationManager
{
	friend class CScene;
	friend class CSceneManager;

protected:
	CNavigationManager3D();
	virtual ~CNavigationManager3D();

protected:
	class CNavigation3D* m_Navigation;
	class CTerrainComponent* m_Terrain;

public:
	virtual void AddNavigationThread(class CNavigationThread* Thread);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Move(class CNavigationAgent* Agent, const Vector3& End);
	virtual float GetHeight(const Vector3& Pos);
};

