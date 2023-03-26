#pragma once
#include "GameObject/GameObject.h"
#include "../UI/PlayerUI.h"

struct PlayerData
{
	int MaxHP; // 최대 HP;
	int CurHP; // 현재 체력
	int Socks; // 양말
	int Spatula; // 뒤집개
	int ShinyFlower; // 반짝이

	PlayerData() 
		: MaxHP(5)
		, CurHP(3)
		, Socks(0)
		, Spatula(0)
		, ShinyFlower(0)
	{
	}

	PlayerData& operator = (const PlayerData& v)
	{
		MaxHP = v.MaxHP;
		CurHP = v.CurHP;
		Socks = v.Socks;
		Spatula = v.Spatula;
		ShinyFlower = v.ShinyFlower;
		return *this;
	}

	bool operator == (const PlayerData& v)	const
	{
		if (MaxHP != v.MaxHP || CurHP != v.CurHP || Socks != v.Socks || Spatula != v.Spatula || ShinyFlower != v.ShinyFlower)
		{
			return false;
		}

		return true;
	}

	bool operator != (const PlayerData& v)	const
	{
		if (MaxHP != v.MaxHP || CurHP != v.CurHP || Socks != v.Socks || Spatula != v.Spatula || ShinyFlower != v.ShinyFlower)
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
	CSharedPtr<class CParticleComponent> m_Particle;

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
	Vector3		m_RespawnPos; //kbj checkpoint
	float m_Speed;
	float m_CameraSpeed;
	int m_KeyCount;
	int m_JumpCount;
	bool m_IsStop;
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
	virtual int InflictDamage(int damage = 1);
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

public :
	int AddHp(); // 0을 반환하는 경우, HP증감이 없었음을 의미함.
	int AddSock();
	int AddSpatula();
	int AddShinyFlower(int Count);
	int AddItem(const EItemList& Item, int Count = 1);

	bool SubSock(int Count);
	bool SubSpatula(int Count);
	bool SubShinyFlower(int Count);

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

	void SetSpatula(int Spatula)
	{
		m_PlayerData.Spatula = Spatula;
		m_PlayerUI->SetFritter(Spatula);
	}

	void SetShinyFlower(int ShinyFlower)
	{
		m_PlayerData.ShinyFlower = ShinyFlower;
		m_PlayerUI->SetGlitter(ShinyFlower);
	}

	void SetRespawnPos(const Vector3& vec)
	{
		m_RespawnPos = vec;
	}

	void SetRespawnPos(float x, float y, float z)
	{
		m_RespawnPos.x = x;
		m_RespawnPos.y = y;
		m_RespawnPos.z = z;
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
	void KeyUp();
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
	void MissileLaunch();

	// Patrick
	void Patrick_PickUp();
	void Patrick_Throw();

	// Change Charater
	void ChangeSpongebob();
	void ChangePatrick();
	void ChangeSandy();


	void DebugF1();
};

