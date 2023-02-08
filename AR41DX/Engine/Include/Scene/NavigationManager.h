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
	std::vector<class CNavigationThread*> m_vecNavThread;

public:
	class CScene* GetScene() const
	{
		return m_Owner;
	}

public:
	virtual void AddNavigationThread(class CNavigationThread* Thread)
	{
		m_vecNavThread.push_back(Thread);
	}

public:
	virtual void Start() = 0;
	virtual bool Init() = 0;
	virtual bool CreateNavigationMesh(class CSceneComponent* Component);
	virtual void Move(class CNavigationAgent* Agent, const Vector2& End);
	virtual void Move(class CNavigationAgent3D* Agent, const Vector3& End);
	virtual float GetHeight(const Vector3& Pos);
	virtual bool GetPickingPos(Vector3& Result)	const;
};

