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

private:
	CSharedPtr<class CColliderCube>		m_DetectArea;
	CSharedPtr<class CColliderCube>		m_AttackArea;

private:
	bool	m_DetectOn;
	bool	m_AttackOn;

public:
	void SetDetectOn(bool DetectOn)
	{
		m_DetectOn = DetectOn;
	}

	bool GetDetectOn()
	{
		return m_DetectOn;
	}

	void SetAttackOn(bool AttackOn)
	{
		m_AttackOn = AttackOn;
	}

	bool GetAttackOn()
	{
		return m_AttackOn;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CFodder* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void WalkAnim();
	void ChaseAnim();
	void AttackAnim();
	void Dead();
	void Debris(); // ¿‹«ÿ∑Œ ∫Ø∞Ê.

private:
	void Collision_ChaseOn(const CollisionResult& result);
	void Release_ChaseOff(const CollisionResult& result);
	void Collision_AttackOn(const CollisionResult& result);
	void Release_AttackOff(const CollisionResult& result);
};