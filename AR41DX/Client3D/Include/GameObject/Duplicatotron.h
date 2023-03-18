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

private:
	CSharedPtr<class CHammer>	m_Hammer;
	bool	m_DetectOn;
	bool	m_SpawnOn;
	int		m_CountHammer;
	float	m_DelayTime;

public:
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

public:
	void Collision_Detect(const CollisionResult& result);
	void Release_DetectOff(const CollisionResult& result);
	void SpawnAnimation();
	void SpawnHammers();
};
