#pragma once

#include "CollectibleItems.h"

class CGoldenSpatula :
	public CCollectibleItems
{
	friend class CScene;

protected:
	CGoldenSpatula();
	CGoldenSpatula(const CGoldenSpatula& Obj);
	virtual ~CGoldenSpatula();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGoldenSpatula* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

