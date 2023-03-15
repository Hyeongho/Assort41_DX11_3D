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
	CSharedPtr<class CColliderOBB3D>	m_DetectArea;
	CSharedPtr<class CColliderOBB3D>	m_AttackArea;
	CSharedPtr<class CColliderOBB3D>	m_BodyCube;
	CSharedPtr<class CColliderOBB3D>	m_WeaponCube;

	CSharedPtr<class CFodderBT>			m_FodderBT;

	CSharedPtr<class CStaticMeshComponent>	m_DebrisMesh1;
	CSharedPtr<class CStaticMeshComponent>	m_DebrisMesh2;
	CSharedPtr<class CStaticMeshComponent>	m_DebrisMesh3;
	CSharedPtr<class CStaticMeshComponent>	m_DebrisMesh4;

private:
	bool	m_DetectOn;
	bool	m_AttackOn;
	bool	m_WeaponAttack; // 무기공격여부.

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
	void Walk();
	void Detect_Chase();
	void Attack();
	void WeaponAttackOn();
	void Dead();
	void Debris(); // 잔해로 변경.

private:
	void Collision_Detect_ChaseOn(const CollisionResult& result);
	void Release_Detect_ChaseOff(const CollisionResult& result);
	void Collision_AttackOn(const CollisionResult& result);
	void Release_AttackOff(const CollisionResult& result);
	void Collision_Body(const CollisionResult& result);

	void Collision_WeaponAttack(const CollisionResult& result);
	void Release_WeaponAttackOff(const CollisionResult& result);

};