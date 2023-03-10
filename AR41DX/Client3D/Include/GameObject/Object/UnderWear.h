#pragma once

#include "CollectibleItems.h"

class CUnderWear :
	public CCollectibleItems
{
	friend class CScene;

protected:
	CUnderWear();
	CUnderWear(const CUnderWear& Obj);
	virtual ~CUnderWear();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CUnderWear* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	virtual void PlayerCollisionItem(const CollisionResult& result);
};

