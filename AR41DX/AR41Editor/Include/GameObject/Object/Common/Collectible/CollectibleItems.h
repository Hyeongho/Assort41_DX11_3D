#pragma once

#include "GameObject\GameObject.h"

enum class EColItemType {
	GoldenSpatula,
	ShinyFlower,
	Sock,
	UnderWear,
	End
};

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
	CSharedPtr<class CStaticMeshComponent>		m_Bubble;
	CSharedPtr<class CColliderOBB3D>			m_Collider;

protected :
	EColItemType	m_ColItemType;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCollectibleItems* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

protected: // Collision
	virtual void Collision_Player(const CollisionResult& result); // 아이템에 닿았을 때 습득 처리
};

