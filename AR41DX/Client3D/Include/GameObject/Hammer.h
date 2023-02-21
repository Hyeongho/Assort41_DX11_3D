#pragma once
#include "Monster.h"

class CHammer :
	public CMonster
{
	friend class CScene;

	CHammer();
	CHammer(const CHammer& Obj);
	virtual ~CHammer();

private:;
	   bool		m_Stunned; // 뚱이 공격시 스턴

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CHammer* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Walk();
	void Chase();
	void Attack();
	void Lassoed(); // 플레이어가 다람이일 때 올가미공격
	void Stunned(); // 플레이어가 뚱이일 때 스턴
	void Dead();
	void Debris();
};