#pragma once

#include "GameObject\GameObject.h"

class CGate :
    public CGameObject
{
	friend class CScene;

protected:
	CGate();
	CGate(const CGate& Obj);
	virtual ~CGate();

private:
	CSharedPtr<class CStaticMeshComponent>		m_ArmMesh;
	CSharedPtr<class CStaticMeshComponent>		m_BottomMesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CColliderCube>				m_ArmCube;
	CSharedPtr<class CColliderCube>				m_BottomCube;
	CSharedPtr<class CColliderCube>				m_DetectRange;

private:
	bool	m_OpenSesameOn;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGate* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Collision_OpenSesame(const CollisionResult& result);
};

