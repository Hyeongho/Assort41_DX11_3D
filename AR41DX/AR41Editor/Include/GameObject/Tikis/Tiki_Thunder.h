#pragma once

#include "TikiBase.h"

class CTiki_Thunder :
	public CTikiBase
{
	friend class CScene;

protected:
	CTiki_Thunder();
	CTiki_Thunder(const CTiki_Thunder& Obj);
	virtual ~CTiki_Thunder();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTiki_Thunder* Clone()    const;
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

