#pragma once

#include "TikiBase.h"

class CTiki_Wood :
	public CTikiBase
{
	friend class CScene;

protected:
	CTiki_Wood();
	CTiki_Wood(const CTiki_Wood& Obj);
	virtual ~CTiki_Wood();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTiki_Wood* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

protected:
	virtual void Tiki_Die();

private :
	void CreateAnim();
	void ChangeAnim_Idle();
	void ChangeAnim_Die();

private : // Collision
	void Collision_PlayerAttack(const CollisionResult& result);
};

