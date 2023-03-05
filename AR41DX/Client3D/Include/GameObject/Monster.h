#pragma once

#include "GameObject\GameObject.h"

#include "FodderBT.h"
#include "HammerBT.h"


enum class EMonsterState
{
	None,
	MonsterWalk,
	MonsterChase,
	MonsterAttack,
	MonsterDead,
	Max
};

class CMonster :
	public CGameObject
{
	friend class CScene;

protected:
	CMonster();
	CMonster(const CMonster& Obj);
	virtual ~CMonster();

protected:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CRigidBody>				m_Rigid;

protected:
	bool	m_DetectRange;
	bool	m_AttackRange;
	float	m_MoveSpeed;
	float	m_DeltaTime;
	int		m_MonsterHP;
	EMonsterState m_MonsterState;

public:
	void SetMoveSpeed(float MoveSpeed)
	{
		m_MoveSpeed = MoveSpeed;
	}

	void SetMoveSpeed(int MonsterHP)
	{
		m_MonsterHP = MonsterHP;
	}

	float GetMoveSpeed()
	{
		return m_MoveSpeed;
	}

	//float GetMonsterHP()
	//{
	//	return m_MonsterHP;
	//}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CMonster* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

