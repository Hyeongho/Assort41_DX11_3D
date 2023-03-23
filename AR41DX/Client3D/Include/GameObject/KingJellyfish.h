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
	float	m_AttackTime; // ���� ��� �� ��Ÿ��
	Vector3	m_MonsterPos; // ���� ��ġ
	Vector3 m_PlayerPos; // �÷��̾� ��ġ
	Vector3	m_PoolPos;	// ����� ��ġ
	float	m_MoveSpeed; // ���� �̵� �ӵ�

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
	void Damage(); // �÷��̾�� ���� ���ϴ� ����
	void Angry(); // �÷��̾�� ���� ���� ���� ��������� �̵�
	void OnGround();
	void Attack(); // ���� ����1, ������ �����ϸ� �� ����� ����ĸ� ����
	void SpawnJellyfish(); // ���� ����2, ���ĸ� ���͸� ����
	void SpawnJellyfish1(); // ���� ����2, ���ĸ� ���͸� ����
	void SpawnJellyfish2(); // ���� ����2, ���ĸ� ���͸� ����
	void SpawnJellyfish3(); // ���� ����2, ���ĸ� ���͸� ����
	void SpawnJellyfish4(); // ���� ����2, ���ĸ� ���͸� ����
	void ReSpawn(); // ��������� �̵�
};

