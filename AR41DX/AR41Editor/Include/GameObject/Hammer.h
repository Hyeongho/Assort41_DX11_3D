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
	   bool		m_Stunned; // ���� ���ݽ� ����

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
	void Lassoed(); // �÷��̾ �ٶ����� �� �ð��̰���
	void Stunned(); // �÷��̾ ������ �� ����
	void Dead();
	void Debris();
};