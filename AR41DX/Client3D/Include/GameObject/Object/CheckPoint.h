#pragma once

#include "GameObject\GameObject.h"

class CCheckPoint :
	public CGameObject
{
	friend class CScene;

protected:
	CCheckPoint();
	CCheckPoint(const CCheckPoint& Obj);
	virtual ~CCheckPoint();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_FlagMesh;
	//CSharedPtr<class CStaticMeshComponent>		m_BottomMesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CColliderOBB3D>				m_CheckPointCube;
	CSharedPtr<class CColliderOBB3D>				m_DetectRange;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCheckPoint* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void CheckPointIdleAnim();

private:
	void Collision_CheckPoint_Active(const CollisionResult& result);
	void Release_CheckPoint_Inactive(const CollisionResult& result);
};

