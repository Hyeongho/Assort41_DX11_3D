#pragma once

#include "GameObject/GameObject.h"

struct PlayerData
{
	int MaxHP; // 최대 HP;
	int CurHP; // 현재 체력
	int Socks; // 양말
	int Fritter; // 뒤집개
	int Glittering; // 반짝이

	PlayerData() : MaxHP(5), CurHP(3), Socks(0), Fritter(0), Glittering(0)
	{

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
	//컴포넌트
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CAnimationMeshComponent>	m_WeaponMesh;
	CSharedPtr<class CCameraComponent> m_Camera;
	CSharedPtr<class CTargetArm> m_Arm;
	CSharedPtr<class CNavigationAgent3D> m_NavAgent;
	CSharedPtr<class CRigidBody> m_Rigid;
	CSharedPtr<class CColliderOBB3D> m_Cube;
	//
	CSharedPtr<class CMesh> m_ReserveMesh[(int)EMain_Character::Max];
	CSharedPtr<class CAnimation> m_Anim[(int)EMain_Character::Max];
	CSharedPtr<class CPlayerUI>	m_PlayerUI;

protected:
	PlayerData m_PlayerData;
	EMain_Character m_MainCharacter;
	float m_Speed;
	float m_CameraSpeed;
	int m_KeyCount;
	int m_JumpCount;
	float m_HoverTime; // 내려찍기 등을 위한 공중부양 시간
	bool m_IsLoading;	//로드 체크용 변수-김범중
	// ========== Patrick 용 ==========
	bool m_IsHolding; // 물건픽업/쓰로우 액션용
	float m_BellyAttackTime;
	bool m_SlamDown;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPlayer* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
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

	int GetMaxHP() const
	{
		return m_PlayerData.MaxHP;
	}

	int GetCurHP() const
	{
		return m_PlayerData.CurHP;
	}

	int GetSocks() const
	{
		return m_PlayerData.Socks;
	}

	int GetFritter() const
	{
		return m_PlayerData.Fritter;
	}

	int GetGlittering() const
	{
		return m_PlayerData.Glittering;
	}

public:
	//공통
	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void Stop();
	void Jump();
	void AttackKey();
	void CameraRotationKey();
	void KeyDown();
	void KeyUp();
	void Interaction();
	void Menu();
	void IngameUI();
	void RClick();
	void LClick(); // Attack
	void ResetIdle();	//아이들상태로 되돌리는 함수

	// Spongebob
	void Headbutt();
	void Missile();

	// Patrick
	void Patrick_BellyAttack();
	void Patrick_BellyAttackMove();
	void Patrick_SlamDown(); // 내려찍기
	void Patrick_PickUp();
	void Patrick_Throw();

	// Sandy

	// Change Charater
	void ChangeSpongebob();
	void ChangePatrick();
	void ChangeSandy();

	// 충돌체 테스트 용
	void CollisionTest(const CollisionResult& result);
};

