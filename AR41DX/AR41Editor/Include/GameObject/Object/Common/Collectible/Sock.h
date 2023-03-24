#pragma once

#include "CollectibleItems.h"

class CSock :
	public CCollectibleItems
{
	friend class CScene;

protected:
	CSock();
	CSock(const CSock& Obj);
	virtual ~CSock();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSock* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	virtual void Collision_Player(const CollisionResult& result); // 아이템에 닿았을 때 습득 처리
};

