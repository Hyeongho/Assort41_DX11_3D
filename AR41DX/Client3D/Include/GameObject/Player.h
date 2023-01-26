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

class CPlayer :
    public CGameObject
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
	CSharedPtr<class CAnimation> m_Animation;

protected:
	PlayerData m_PlayerData;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPlayer* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

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
	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void Jump();
	void AttackKey();
	void CameraRotationKey();
};

