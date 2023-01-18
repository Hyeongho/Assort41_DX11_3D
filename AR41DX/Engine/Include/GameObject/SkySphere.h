#pragma once

#include "GameObject.h"

class CSkySphere :
    public CGameObject
{
	friend class CScene;

protected:
	CSkySphere();
	CSkySphere(const CSkySphere& Obj);
	virtual ~CSkySphere();

private:
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CMaterial>	m_Material;

public:
	virtual void Start();
	virtual bool Init();
	virtual CSkySphere* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

