#pragma once

#include "GameObject\GameObject.h"

class CTeleportBox :
	public CGameObject
{
	friend class CScene;

protected:
	CTeleportBox();
	CTeleportBox(const CTeleportBox& Obj);
	virtual ~CTeleportBox();

private:
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CAnimation>	m_Animation;
	CSharedPtr<class CColliderCube>	m_BoxWallCube1;
	CSharedPtr<class CColliderCube>	m_BoxWallCube2;
	CSharedPtr<class CColliderCube>	m_BoxWallCube3;
	CSharedPtr<class CColliderCube>	m_BoxWallCube4;
	CSharedPtr<class CColliderCube>	m_BoxBottomCube;

	CSharedPtr<class CColliderCube>	m_DetectRange;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTeleportBox* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Collision_OpenBox(const CollisionResult& result);
};

