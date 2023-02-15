
#pragma once
#include "Monster.h"

class CHammer :
	public CMonster
{
	friend class CScene;

	CHammer();
	CHammer(const CHammer& Obj);
	virtual ~CHammer();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CHammer* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

