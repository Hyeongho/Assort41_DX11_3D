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
	CSharedPtr<class CAnimation>		m_Animation;

private:
	float	m_AttackTime; // 공격 모션 중 쿨타임 

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CKingJellyfish* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void Damage(); // 플레이어에게 공격 당하는 순간
	void Angry(); // 플레이어에게 공격 당한 이후
	void OnGround(); // 공격 패턴1, 땅으로 착지하며 원 모양의 충격파를 날림
	void Attack(); // 공격 패턴2, 땅에 착지한 상태로 원 모양의 충격파를 날림
	void SpawnJellyfish(); // 공격 패턴2, 해파리 몬스터를 생성
};

