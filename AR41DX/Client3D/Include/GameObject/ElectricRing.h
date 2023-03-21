#pragma once

#include "GameObject\GameObject.h"

class CElectricRing :
	public CGameObject
{
    friend class CScene;
    friend class CKingJellyfish;

protected:
	CElectricRing();
	CElectricRing(const CElectricRing& Obj);
	virtual ~CElectricRing();

private:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CColliderOBB3D>	m_Collider;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CElectricRing* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

