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

private:
	CSharedPtr<class CStaticMeshComponent>		m_FodderDebrisMesh1;
	CSharedPtr<class CStaticMeshComponent>		m_FodderDebrisMesh2;
	CSharedPtr<class CStaticMeshComponent>		m_FodderDebrisMesh3;
	CSharedPtr<class CStaticMeshComponent>		m_FodderDebrisMesh4;
	//CSharedPtr<class CColliderOBB3D>			m_Cube;

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



