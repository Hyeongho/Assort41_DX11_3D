#pragma once

#include "GameObject\GameObject.h"

class CCBL_Floor
	: public CGameObject
{
	friend class CScene;

protected:
	CCBL_Floor();
	CCBL_Floor(const CCBL_Floor& Obj);
	virtual ~CCBL_Floor();

protected:
	CSharedPtr<class CStaticMeshComponent>	m_InnerMesh;
	CSharedPtr<class CStaticMeshComponent>	m_OuterMesh;
	//CSharedPtr<class CColliderOBB3D>		m_Collider;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCBL_Floor* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

