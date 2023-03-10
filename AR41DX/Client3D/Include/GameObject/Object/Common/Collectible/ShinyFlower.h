#pragma once

#include "CollectibleItems.h"

class CShinyFlower :
	public CCollectibleItems
{
	friend class CScene;

protected:
	CShinyFlower();
	CShinyFlower(const CShinyFlower& Obj);
	virtual ~CShinyFlower();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CShinyFlower* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	virtual void PlayerCollisionItem(const CollisionResult& result);
};

