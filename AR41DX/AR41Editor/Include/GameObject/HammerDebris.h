#pragma once

#include "Monster.h"

class CHammerDebris :
	public CMonster
{
	friend class CScene;

protected:
	CHammerDebris();
	CHammerDebris(const CHammerDebris& Obj);
	virtual ~CHammerDebris();

protected:
	CSharedPtr<class CStaticMeshComponent>		m_HammerDebrisMesh1;
	CSharedPtr<class CStaticMeshComponent>		m_HammerDebrisMesh2;
	CSharedPtr<class CStaticMeshComponent>		m_HammerDebrisMesh3;
	CSharedPtr<class CStaticMeshComponent>		m_HammerDebrisMesh4;

	CSharedPtr<class CColliderOBB3D>			m_Mesh1Cube;
	CSharedPtr<class CColliderOBB3D>			m_Mesh2Cube;
	CSharedPtr<class CColliderOBB3D>			m_Mesh3Cube;
	CSharedPtr<class CColliderOBB3D>			m_Mesh4Cube;

	CSharedPtr<class CRigidBody>	m_Rigid1;
	CSharedPtr<class CRigidBody>	m_Rigid2;
	CSharedPtr<class CRigidBody>	m_Rigid3;
	CSharedPtr<class CRigidBody>	m_Rigid4;



private:
	bool	m_DefyingGravity;
	CHammer* m_Hammer;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CHammerDebris* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	CStaticMeshComponent* SetHammerDebrisMesh1(CStaticMeshComponent* HammerDebrisMesh1)
	{
		m_HammerDebrisMesh1 = HammerDebrisMesh1;
	}

	CStaticMeshComponent* SetHammerDebrisMesh2(CStaticMeshComponent* HammerDebrisMesh2)
	{
		m_HammerDebrisMesh2 = HammerDebrisMesh2;
	}

	CStaticMeshComponent* SetHammerDebrisMesh3(CStaticMeshComponent* HammerDebrisMesh3)
	{
		m_HammerDebrisMesh3 = HammerDebrisMesh3;
	}

	CStaticMeshComponent* SetHammerDebrisMesh4(CStaticMeshComponent* HammerDebrisMesh4)
	{
		m_HammerDebrisMesh4 = HammerDebrisMesh4;
	}

private:
	void Destroy();
};



