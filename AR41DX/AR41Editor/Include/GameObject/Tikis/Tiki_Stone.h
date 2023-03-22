#pragma once

#include "TikiBase.h"

class CTiki_Stone :
	public CTikiBase
{
	friend class CScene;

protected:
	CTiki_Stone();
	CTiki_Stone(const CTiki_Stone& Obj);
	virtual ~CTiki_Stone();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTiki_Stone* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

protected:
	virtual void Tiki_Die();

private:
	void CreateAnim();
	void ChangeAnim_Idle();
	void ChangeAnim_Die();

private: // Collision
	void Collision_PlayerAttack(const CollisionResult& result);
};