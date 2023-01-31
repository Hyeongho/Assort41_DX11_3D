#pragma once

#include "NavigationManager.h"

class CNavigationManager2D :
    public CNavigationManager
{
	friend class CScene;
	friend class CSceneManager;

protected:
	CNavigationManager2D();
	virtual ~CNavigationManager2D();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Move(class CNavigationAgent* Agent, const Vector2& End);
};

