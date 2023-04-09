#pragma once

#include "GameObject\GameObject.h"
#include "Collectible_Bubble.h"

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

	CSharedPtr<class CCollectible_Bubble>		m_Bubble;

protected :
	EItemList	m_ColItemType;


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

