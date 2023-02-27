#pragma once

#include "GameObject\GameObject.h"

enum class EBossList {
	KingJellyFish,
	RoboSponge,
	RoboSandy,
	FlyingDutchman,
	End
};

struct BossData
{
	EBossList Boss;

	int MaxHP;
	int CurHP;

	bool ActionStart;

	BossData() :
		Boss(EBossList::KingJellyFish),
		MaxHP(5),
		CurHP(MaxHP),
		ActionStart(false)
	{}
};

class CBossMonster :
	public CGameObject
{
	friend class CScene;

protected:
	CBossMonster();
	CBossMonster(const CBossMonster& Obj);
	virtual ~CBossMonster();

protected:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CUIWindow>					m_BossUI;

	BossData* m_BossData;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CBossMonster* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public :
	void SetBossData_MaxHp(int MaxHp);
	void SetBossData(const BossData& BossData);
	void CreateBossUI();
	
};

