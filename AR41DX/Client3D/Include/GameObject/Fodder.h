#pragma once

#include "Monster.h"

class CFodder :
	public CMonster
{
	friend class CScene;

protected:
	CFodder();
	CFodder(const CFodder& Obj);
	virtual ~CFodder();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CFodder* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Walk();
	void Chase();
	void Attack();
	void Dead();
	void Debris(); // ¿‹«ÿ∑Œ ∫Ø∞Ê.
};