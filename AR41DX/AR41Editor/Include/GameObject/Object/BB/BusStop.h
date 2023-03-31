#pragma once

#include "GameObject\GameObject.h"

class CBusStop
	: public CGameObject
{
	friend class CScene;

protected:
	CBusStop();
	CBusStop(const CBusStop& Obj);
	virtual ~CBusStop();

protected:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CTargetArm> m_Arm;
	CSharedPtr<class CCameraComponent> m_Camera;

private:
	EMapList	m_PurposeScene;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CBusStop* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public :
	void CallBus();
	void CutSceneStart();

public:
	void SetPurposeScene(const EMapList& Map)
	{
		m_PurposeScene = Map;
	}
};

