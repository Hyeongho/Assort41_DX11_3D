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

public:
	virtual void Start();
	virtual bool Init();
	virtual void Move(class CNavigationAgent* Agent, const Vector2& End);
};

