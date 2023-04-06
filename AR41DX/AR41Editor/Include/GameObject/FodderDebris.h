#pragma once

#include "Monster.h"


class CFodderDebris :
	public CMonster
{
	friend class CScene;

protected:
	CFodderDebris();
	CFodderDebris(const CFodderDebris& Obj);
	virtual ~CFodderDebris();

protected:
	CSharedPtr<class CStaticMeshComponent>		m_FodderDebrisMesh1;
	CSharedPtr<class CStaticMeshComponent>		m_FodderDebrisMesh2;
	CSharedPtr<class CStaticMeshComponent>		m_FodderDebrisMesh3;
	CSharedPtr<class CStaticMeshComponent>		m_FodderDebrisMesh4;

	CSharedPtr<class CColliderOBB3D>			m_Mesh1Cube;
	CSharedPtr<class CColliderOBB3D>			m_Mesh2Cube;
	CSharedPtr<class CColliderOBB3D>			m_Mesh3Cube;
	CSharedPtr<class CColliderOBB3D>			m_Mesh4Cube;

	CSharedPtr<class CRigidBody>	m_Rigid1;
	CSharedPtr<class CRigidBody>	m_Rigid2;
	CSharedPtr<class CRigidBody>	m_Rigid3;
	CSharedPtr<class CRigidBody>	m_Rigid4;

	//
	float m_Velocity[3];
	float m_Acceleration[3];
	float m_Mass;
	bool  m_IsPopped;
	bool	m_DefyingGravity;
	CFodder* m_Fodder;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CFodderDebris* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	CStaticMeshComponent* SetFodderDebrisMesh1(CStaticMeshComponent* FodderDebrisMesh1)
	{
		m_FodderDebrisMesh1 = FodderDebrisMesh1;
	}

	CStaticMeshComponent* SetFodderDebrisMesh2(CStaticMeshComponent* FodderDebrisMesh2)
	{
		m_FodderDebrisMesh2 = FodderDebrisMesh2;
	}

	CStaticMeshComponent* SetFodderDebrisMesh3(CStaticMeshComponent* FodderDebrisMesh3)
	{
		m_FodderDebrisMesh3 = FodderDebrisMesh3;
	}

	CStaticMeshComponent* SetFodderDebrisMesh4(CStaticMeshComponent* FodderDebrisMesh4)
	{
		m_FodderDebrisMesh4 = FodderDebrisMesh4;
	}

private:
	void Destroy();
};



