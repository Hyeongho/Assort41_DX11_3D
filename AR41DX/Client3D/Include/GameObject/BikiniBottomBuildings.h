
#pragma once

#include "GameObject\GameObject.h"

class CBikiniBottomBuildings :
    public CGameObject
{
	friend class CScene;

protected:
	CBikiniBottomBuildings();
	CBikiniBottomBuildings(const CBikiniBottomBuildings& Obj);
	virtual ~CBikiniBottomBuildings();

private:
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CRigidBody> m_Rigid;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CBikiniBottomBuildings* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

