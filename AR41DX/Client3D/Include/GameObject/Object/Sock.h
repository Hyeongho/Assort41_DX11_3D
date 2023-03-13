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
	virtual void PlayerCollisionItem(const CollisionResult& result);
};

