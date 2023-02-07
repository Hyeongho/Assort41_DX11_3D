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
	void BellyAttack(); // ��ġ��(�⺻����)
	void SlamDown(); // �������
	void PickUp(); // �ݱ�
	void Throw(); // ������
	void Death();
};

