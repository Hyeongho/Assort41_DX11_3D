
#pragma once

#include "GameObject/GameObject.h"

class CSpongebobHouse :
	public CGameObject
{
	friend class CScene;

protected:
	CSpongebobHouse();
	CSpongebobHouse(const CSpongebobHouse& Obj);
	virtual ~CSpongebobHouse();

private:
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CRigidBody> m_Rigid;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSpongebobHouse* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

