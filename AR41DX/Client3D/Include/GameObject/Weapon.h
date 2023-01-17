#pragma once

#include "GameObject\GameObject.h"

class CWeapon :
    public CGameObject
{
	friend class CScene;

protected:
	CWeapon();
	CWeapon(const CWeapon& Obj);
	virtual ~CWeapon();

private:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CWeapon* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

