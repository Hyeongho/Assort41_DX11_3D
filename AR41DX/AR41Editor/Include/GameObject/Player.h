#pragma once

#include "GameObject/GameObject.h"

struct PlayerData
{
	int MaxHP; // �ִ� HP;
	int CurHP; // ���� ü��
	int Socks; // �縻
	int Fritter; // ������
	int Glittering; // ��¦��

	PlayerData() : MaxHP(5), CurHP(3), Socks(0), Fritter(0), Glittering(0)
	{

	}
	PlayerData& operator = (const PlayerData& v)
	{
		MaxHP = v.MaxHP;
		CurHP = v.CurHP;
		Socks = v.Socks;
		Fritter = v.Fritter;
		Glittering = v.Glittering;
		return *this;
	}
	bool operator == (const PlayerData& v)	const
	{
		if (MaxHP != v.MaxHP || CurHP != v.CurHP || Socks != v.Socks || Fritter != v.Fritter || Glittering != v.Glittering)
		{
			return false;
		}
		return true;
	}
	bool operator != (const PlayerData& v)	const
	{
		if (MaxHP != v.MaxHP || CurHP != v.CurHP || Socks != v.Socks || Fritter != v.Fritter || Glittering != v.Glittering)
		{
			return true;
		}
		return false;
	}
};

enum class EMain_Character
{
	Spongebob,
	Patrick,
	Sandy,
	Max,
};

class CPlayer :
	public CGameObject
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& Obj);
	virtual ~CPlayer();

protected:
	//������Ʈ
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CAnimationMeshComponent>	m_WeaponMesh;
	CSharedPtr<class CCameraComponent> m_Camera;
	CSharedPtr<class CTargetArm> m_Arm;
	CSharedPtr<class CNavigationAgent3D> m_NavAgent;
	CSharedPtr<class CRigidBody> m_Rigid;
	CSharedPtr<class CColliderCube> m_Cube;
	CSharedPtr<class CColliderCube> m_HeadCube;	//�������� �밡����
	//
	CSharedPtr<class CMesh> m_ReserveMesh[(int)EMain_Character::Max];
	CSharedPtr<class CAnimation> m_Anim[(int)EMain_Character::Max];
	CSharedPtr<class CPlayerUI>	m_PlayerUI;
	CSharedPtr<class CPauseUI>	m_PauseUI;

protected:
	PlayerData m_PlayerData;
	PlayerData m_LoadData;
	EMain_Character m_MainCharacter;
	float m_Speed;
	float m_CameraSpeed;
	int m_KeyCount;
	int m_JumpCount;
	float m_HoverTime; // ������� ���� ���� ���ߺξ� �ð�
	bool m_IsLoading;	//�ε� üũ�� ����-�����
	bool m_IsDoubleJump;	//�������� -�����
	// ========== Patrick �� ==========
	bool m_IsHolding; // �����Ⱦ�/���ο� �׼ǿ�
	float m_BellyAttackTime;
	bool m_SlamDown;

public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPlayer* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	bool SaveCharacter();
	bool LoadCharacter();

private:
	void LoadSpongebobAnim(); // �������� ���ҽ�
	void LoadPatrickAnim(); // ���� ���ҽ�
	void LoadSandyAnim(); // �ٶ��� ���ҽ�

public:
	void SetPlayerData(PlayerData Playerdata)
	{
		m_PlayerData = Playerdata;
	}

	void SetMaxHP(int HP)
	{
		m_PlayerData.MaxHP = HP;
	}

	void SetCurHP(int HP)
	{
		m_PlayerData.CurHP = HP;
	}

	void SetSocks(int Socks)
	{
		m_PlayerData.Socks = Socks;
	}

	void SetFritter(int Fritter)
	{
		m_PlayerData.Fritter = Fritter;
	}

	void SetGlittering(int Glittering)
	{
		m_PlayerData.Glittering = Glittering;
	}

public:
	PlayerData GetPlayerData() const
	{
		return m_PlayerData;
	}

public:
	//����
	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void Stop();
	void Jump();
	void JumpCheck();
	void AttackKey();
	void CameraRotationKey();
	void KeyDown();
	void KeyUp();
	void Interaction();
	void Menu();
	void IngameUI();
	void RClick();
	void LClick(); // Attack
	void StartBash();	//���� ���� �Լ�
	void ResetIdle();	//���̵���·� �ǵ����� �Լ�

	// Spongebob
	void Headbutt();
	void Missile();
	void Bowl();
	void BowlThrow();

	// Patrick
	void Patrick_BellyAttack();
	void Patrick_BellyAttackMove();
	void Patrick_SlamDown(); // �������
	void Patrick_PickUp();
	void Patrick_Throw();

	// Sandy

	// Change Charater
	void ChangeSpongebob();
	void ChangePatrick();
	void ChangeSandy();
};

