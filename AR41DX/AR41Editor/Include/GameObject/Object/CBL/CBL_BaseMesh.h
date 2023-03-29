#pragma once

#include "GameObject\GameObject.h"

class CCBL_BaseMesh
	: public CGameObject
{
	friend class CScene;

protected:
	CCBL_BaseMesh();
	CCBL_BaseMesh(const CCBL_BaseMesh& Obj);
	virtual ~CCBL_BaseMesh();

protected:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCBL_BaseMesh* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

