#pragma once
#include "Monster.h"

class CHammer :
	public CMonster
{
	friend class CScene;

protected:
	CHammer();
	CHammer(const CHammer& Obj);
	virtual ~CHammer();

private:
	CSharedPtr<class CColliderOBB3D>	m_DetectArea;
	CSharedPtr<class CColliderOBB3D>	m_AttackArea;
	CSharedPtr<class CColliderOBB3D>	m_BodyCube;
	CSharedPtr<class CColliderOBB3D>	m_WeaponCube;
	CSharedPtr<class CRigidBody>		m_Rigid;

	CSharedPtr<class CFodderBT>			m_FodderBT;

	CSharedPtr<class CStaticMeshComponent>	m_DebrisMesh1;
	CSharedPtr<class CStaticMeshComponent>	m_DebrisMesh2;
	CSharedPtr<class CStaticMeshComponent>	m_DebrisMesh3;
	CSharedPtr<class CStaticMeshComponent>	m_DebrisMesh4;

	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;

private:
	bool	m_DetectOn;
	bool	m_AttackOn;
	bool	m_WeaponAttack; // 무기공격여부.
	bool	m_DebrisOn;
	bool	m_Dead;

public:
	void SetDead(bool Dead)
	{
		m_Dead = Dead;
	}

	bool GetDead()
	{
		return m_Dead;
	}

	void SetDebrisOn(bool DebrisOn)
	{
		m_DebrisOn = DebrisOn;
	}

	bool GetDebrisOn()
	{
	  return m_DebrisOn;
	}

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
	virtual CHammer* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Walk();
	void Detect_Chase();
	void Attack();
	//void Lassoed(); // 플레이어가 다람이일 때 올가미공격
	//void Stunned(); // 플레이어가 뚱이일 때 스턴
	void WeaponAttackOn();
	void Dead();
	void Debris();

private:
	void Collision_Detect_ChaseOn(const CollisionResult& result);
	void Release_Detect_ChaseOff(const CollisionResult& result);
	void Collision_AttackOn(const CollisionResult& result);
	void Release_AttackOff(const CollisionResult& result);
	void Collision_Body(const CollisionResult& result);

	void Collision_WeaponAttack(const CollisionResult& result);
	void Release_WeaponAttackOff(const CollisionResult& result);
};