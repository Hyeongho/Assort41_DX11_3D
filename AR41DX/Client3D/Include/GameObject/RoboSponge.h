#pragma once

#include "GameObject/GameObject.h"

struct BossData
{
	int MaxHP;
	int CurHP;

	BossData() :
		MaxHP(5),
		CurHP(MaxHP)
	{}
};

class CRoboSponge :
	public CGameObject
{
	friend class CScene;

protected:
	CRoboSponge();
	CRoboSponge(const CRoboSponge& Obj);
	virtual ~CRoboSponge();

protected:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;

protected:
	BossData m_BossData;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CRoboSponge* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void AttackHorizL();
	void AttackHorizLPose();
	void AttackHorizR();
	void AttackHorizRPose();
	void AttackVerticLLoop();
	void AttackVerticLStart();
	void AttackVerticRLoop();
	void AttackVerticRStart();
	void AttackWordsLoop();
	void AttackWordsStart();
	void Hit1();
	void Hit2();
	void Hit2Pose();
	void Idle();
	void VerticLHold();
	void VerticRHold();
	void Victory();
};

