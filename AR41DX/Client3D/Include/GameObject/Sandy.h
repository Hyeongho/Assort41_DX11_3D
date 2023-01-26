#pragma once

#include "Player.h"

class CSandy :
    public CPlayer
{
	friend class CScene;

protected:
	CSandy();
	CSandy(const CSandy& Obj);
	virtual ~CSandy();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPlayer* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

