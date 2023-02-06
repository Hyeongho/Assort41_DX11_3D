#pragma once

#include "Player.h"

class CPatrick :
	public CPlayer
{
	friend class CScene;

protected:
	CPatrick();
	CPatrick(const CPatrick& Obj);
	virtual ~CPatrick();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPatrick* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	virtual void MoveFront();
	virtual void MoveBack();
	virtual void MoveLeft();
	virtual void MoveRight();

public:
	void LButton(); // Push
	void RButton(); // SlamDown, PickUp, Throw

	void Jump();
	void BellyAttack(); // 밀치기(기본공격)
	void SlamDown(); // 내려찍기
	void PickUp(); // 줍기
	void Throw(); // 던지기
	void Death();
};

