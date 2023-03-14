#pragma once
#include "GameObject/GameObject.h"
#include "../UI/PlayerUI.h"

struct PlayerData
{
	int MaxHP; // 최대 HP;
	int CurHP; // 현재 체력
	int Socks; // 양말
	int Fritter; // 뒤집개
	int Glittering; // 반짝이
	Vector3		RespawnPos; //kbj checkpoint

	PlayerData() 
		: MaxHP(5)
		, CurHP(3)
		, Socks(0)
		, Fritter(0)
		, Glittering(0)
	{
	}
	PlayerData& operator = (const PlayerData& v)
	{
		MaxHP = v.MaxHP;
		CurHP = v.CurHP;
		Socks = v.Socks;
		Fritter = v.Fritter;
		Glittering = v.Glittering;
		RespawnPos = v.RespawnPos;
		return *this;
	}
	bool operator == (const PlayerData& v)	const
	{
		if (MaxHP != v.MaxHP || CurHP != v.CurHP || Socks != v.Socks || Fritter != v.Fritter || Glittering != v.Glittering
			|| RespawnPos!= v.RespawnPos)
		{
			return false;
		}
		return true;
	}
	bool operator != (const PlayerData& v)	const
	{
		if (MaxHP != v.MaxHP || CurHP != v.CurHP || Socks != v.Socks || Fritter != v.Fritter || Glittering != v.Glittering
			|| RespawnPos != v.RespawnPos)
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

enum class EKeyDir
{
	Up= 0x1,  // hex for 0000 0001 
	Down = 0x2,  // hex for 0000 0010
	Left = 0x4,  // hex for 0000 0100
	Right = 0x8,  // hex for 0000 1000
};

class CPlayer : public CGameObject
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& Obj);
	virtual ~CPlayer();

protected:
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CCameraComponent> m_Camera;
	CSharedPtr<class CTargetArm> m_Arm;
	CSharedPtr<class CRigidBody> m_Rigid;
	CSharedPtr<class CColliderOBB3D> m_Cube;
	CSharedPtr<class CColliderCube> m_HeadCube;	//spongebob head
	CSharedPtr<class CColliderCube> m_TailCube;	//spongebob bash
	CSharedPtr<class CColliderOBB3D> m_FrontCube;	//atk

	CSharedPtr<class CWeapon>	m_Weapon;
	CSharedPtr<class CMesh> m_ReserveMesh[(int)EMain_Character::Max];
	CSharedPtr<class CAnimation> m_Anim[(int)EMain_Character::Max];
	CSharedPtr<class CPlayerUI>	m_PlayerUI;
	CSharedPtr<class CPauseUI>	m_PauseUI;

protected:
	PlayerData m_PlayerData;
	PlayerData m_LoadData;
	EMain_Character m_MainCharacter;
	CollisionResult m_WallCollision;	//벽이랑 부딪히고 있는경우
	float m_Speed;
	float m_CameraSpeed;
	int m_KeyCount;
	int m_JumpCount;
	bool m_IsLoading;	//load check kbj
	bool m_IsDoubleJump;	
	bool m_OnCollision;
	// ========== Patrick ==========
	float m_BellyAttackTime;
	bool  m_CanPickUp;
	// ========== sandy ==========
	float m_SpaceTime;
	float m_LassoDistance;
public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPlayer* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	virtual int InflictDamage(int damage);
	virtual void Reset();
	bool SaveCharacter();
	bool LoadCharacter();

private:
	void LoadCheck();
	void CollisionCube(const CollisionResult& result);
	void CollisionTest(const CollisionResult& result);	// 충돌체 테스트 용
	void CollisionTestOut(const CollisionResult& result);
	void LoadSpongebobAnim(); // 스폰지밥 리소스
	void LoadPatrickAnim(); // 뚱이 리소스
	void LoadSandyAnim(); // 다람이 리소스

public:
	void SetPlayerData(PlayerData Playerdata)
	{
		m_PlayerData = Playerdata;
	}

	void SetMaxHP(int HP)
	{
		m_PlayerData.MaxHP = HP;
		m_PlayerUI->SetMaxHp(HP);
	}

	void SetCurHP(int HP)
	{
		m_PlayerData.CurHP = HP;
		m_PlayerUI->SetHp(HP);
	}

	void SetSocks(int Socks)
	{
		m_PlayerData.Socks = Socks;
		m_PlayerUI->SetSocks(Socks);
	}

	void SetFritter(int Fritter)
	{
		m_PlayerData.Fritter = Fritter;
		m_PlayerUI->SetFritter(Fritter);
	}

	void SetGlittering(int Glittering)
	{
		m_PlayerData.Glittering = Glittering;
		m_PlayerUI->SetGlitter(Glittering);
	}

	void SetRespawnPos(const Vector3& vec)
	{
		m_PlayerData.RespawnPos = vec;
	}
	void SetRespawnPos(float x, float y, float z)
	{
		m_PlayerData.RespawnPos.x = x;
		m_PlayerData.RespawnPos.y = y;
		m_PlayerData.RespawnPos.z = z;
	}

	void SetCanPickUp(bool b)
	{
		m_CanPickUp = b;
	}

public:
	PlayerData GetPlayerData() const
	{
		return m_PlayerData;
	}

public:
	//공통
	void KeyDown();
	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void KeyUpUp();
	void KeyDownUp();
	void KeyLeftUp();
	void KeyRightUp();
	void JumpDown();
	void JumpPush(); 	// Sandy
	void JumpUp(); 		// Sandy
	void JumpCheck();
	void CameraRotationKey();
	void IngameUI();	//tab
	void LClick(); // Attack
	void RClickDown();
	void RClickPush();
	void RClickUp();
	void StartBash();
	void BashCheck();
	void ResetIdle();

	// Spongebob
	void Headbutt();
	void Missile();

	// Patrick
	void Patrick_Throw();

	// Change Charater
	void ChangeSpongebob();
	void ChangePatrick();
	void ChangeSandy();
};

