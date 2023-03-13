#pragma once

#include "GameObject\GameObject.h"

class CJumpTree :
    public CGameObject
{
	friend class CScene;

protected:
	CJumpTree();
	CJumpTree(const CJumpTree& Obj);
	virtual ~CJumpTree();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_TopMesh;
	CSharedPtr<class CStaticMeshComponent>		m_BottomMesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CColliderCube>				m_TopCube;
	CSharedPtr<class CColliderCube>				m_BottomCube;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CJumpTree* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Collision_Idle(const CollisionResult& result);
	void Release_Bounce(const CollisionResult& result);
};

