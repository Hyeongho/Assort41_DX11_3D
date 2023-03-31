#pragma once

#include "Monster.h"

class CDuplicatotron :
	public CMonster
{
	friend class CScene;

protected:
	CDuplicatotron();
	CDuplicatotron(const CDuplicatotron& Obj);
	virtual ~CDuplicatotron();


private:
	CSharedPtr<class CColliderOBB3D>	m_DetectArea;
	CSharedPtr<class CColliderOBB3D>	m_BodyCube;
	CSharedPtr<class CRigidBody>		m_Rigid;
	CSharedPtr<class CHammer>			m_Hammer;
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	

private:
	bool	m_DetectOn;
	bool	m_SpawnOn;
	int		m_CountCan;
	float	m_DelayTime;

public:
	void AddCountCan(int Num)
	{
		m_CountCan += Num;
	}
	
	void SetSpawnOn(bool SpawnOn)
	{
		m_SpawnOn = SpawnOn;
	}

	void SetCountCan(int CountCan)
	{
		m_CountCan = CountCan;
	}

	int GetCountCan()
	{
	   return m_CountCan;
	}

	void SetDetectOn(bool DetectOn)
	{
		m_DetectOn = DetectOn;
	}

	bool GetDetectOn()
	{
		return m_DetectOn;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CDuplicatotron* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Collision_Detect(const CollisionResult& result);
	void Release_DetectOff(const CollisionResult& result);
	void Collision_Dead(const CollisionResult& result);
	void Destroyed();
	void SpawnAnimation();
	void SpawnCan();
	void IdleSound();
	void SpawnSound();
};
