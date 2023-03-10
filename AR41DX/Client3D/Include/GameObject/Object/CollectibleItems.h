#pragma once

#include "GameObject\GameObject.h"

class CCollectibleItems
	: public CGameObject
{
	friend class CScene;

protected:
	CCollectibleItems();
	CCollectibleItems(const CCollectibleItems& Obj);
	virtual ~CCollectibleItems();

protected:
	CSharedPtr<class CStaticMeshComponent>		m_Mesh;
	CSharedPtr<class CColliderOBB3D>			m_Collider;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCollectibleItems* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

