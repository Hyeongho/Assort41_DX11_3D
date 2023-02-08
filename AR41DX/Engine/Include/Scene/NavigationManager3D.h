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
	class CNavigationMesh* m_NavigationMesh;
	class CTerrainComponent* m_Terrain;

public:
	virtual void AddNavigationThread(class CNavigationThread* Thread);

public:
	virtual void Start();
	virtual bool Init();
	virtual bool CreateNavigationMesh(class CSceneComponent* Component);
	virtual void Move(class CNavigationAgent3D* Agent, const Vector3& End);
	virtual float GetHeight(const Vector3& Pos);
	virtual bool GetPickingPos(Vector3& Result)	const;
};

