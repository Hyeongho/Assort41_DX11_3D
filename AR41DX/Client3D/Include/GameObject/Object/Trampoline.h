#pragma once

#include "GameObject\GameObject.h"

class CTrampoline
	: public CGameObject
{
	friend class CScene;

protected:
	CTrampoline();
	CTrampoline(const CTrampoline& Obj);
	virtual ~CTrampoline();

protected:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CColliderCube>			m_Cube;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTrampoline* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

