#pragma once
#include "GameObject\GameObject.h"

class CKingJellyfish :
	public CGameObject
{
	friend class CScene;
protected:
	CKingJellyfish();
	CKingJellyfish(const CKingJellyfish& Obj);
	virtual ~CKingJellyfish();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimationMeshComponent>	m_EffectMesh;
	CSharedPtr<class CAnimation>		m_Animation;
	CSharedPtr<class CAnimation>		m_EffectMeshAnimation;
	CSharedPtr<class CColliderOBB3D>	m_AttackCollision;
	CSharedPtr<class CColliderOBB3D>	m_DetectCollision;
	CSharedPtr<class CColliderOBB3D>	m_PoolCollision;
	CSharedPtr<class CElectricRing>		m_ElectricRing;
	class CJellyfish*					m_Jellyfish[6];
	CSharedPtr<class CPlayer>			m_Player;

public:
	bool			m_Attack;
	int				m_CollisionCount;
	bool			m_Collision;
	bool			m_Angry;
	bool			m_Idle;
	bool			m_SpawnJellyfish;
	bool			m_OnGround;
	bool			m_Electric;
	bool			m_Damage;
	bool			m_Pool;
	bool			m_ReSpawn;

public:
	float	m_AttackTime; // 공격 모션 중 쿨타임
	Vector3	m_MonsterPos; // 몬스터 위치
	Vector3 m_PlayerPos; // 플레이어 위치
	Vector3	m_PoolPos;	// 목욕탕 위치
	float	m_MoveSpeed; // 몬스터 이동 속도

public:
	void SetMoveSpeed(float MoveSpeed)
	{
		m_MoveSpeed = MoveSpeed;
	}

	float GetMoveSpeed() const
	{
		return m_MoveSpeed;
	}


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CKingJellyfish* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	void AttackCollision(const CollisionResult& result);
	void DetectCollision(const CollisionResult& result);
	void PoolCollision(const CollisionResult& result);

public:
	void Idle();
	void Damage(); // 플레이어에게 공격 당하는 순간
	void Angry(); // 플레이어에게 공격 당한 이후 목욕탕으로 이동
	void OnGround();
	void Attack(); // 공격 패턴1, 땅으로 착지하며 원 모양의 충격파를 날림
	void SpawnJellyfish(); // 공격 패턴2, 해파리 몬스터를 생성
	void SpawnJellyfish1(); // 공격 패턴2, 해파리 몬스터를 생성
	void SpawnJellyfish2(); // 공격 패턴2, 해파리 몬스터를 생성
	void SpawnJellyfish3(); // 공격 패턴2, 해파리 몬스터를 생성
	void SpawnJellyfish4(); // 공격 패턴2, 해파리 몬스터를 생성
	void ReSpawn(); // 목욕탕으로 이동
};

