#pragma once

#include "GameObject\GameObject.h"

class CJellyfish :
	public CGameObject
{
	friend class CScene;

protected:
	CJellyfish();
	CJellyfish(const CJellyfish& Obj);
	virtual ~CJellyfish();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>		m_Animation;

	CSharedPtr<class CColliderCube> m_Cube;
	CSharedPtr<class CColliderCube> m_Detect;

	Vector3	m_MonsterPos;
	bool	m_Chase;

public:
	int	m_Count;
	void SetCount(int Count)
	{
		m_Count = Count;
	}

	bool	m_Boss;
	void SetBoss(bool Boss)
	{
		m_Boss = Boss;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CJellyfish* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Collision(const CollisionResult& result);
	void DetectCollision(const CollisionResult& result);
	void Dead();
};

