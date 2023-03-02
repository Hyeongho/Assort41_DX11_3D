
#pragma once

#include "GameObject\GameObject.h"

class CJellyfishFieldObj :
	public CGameObject
{
	friend class CScene;

protected:
	CJellyfishFieldObj();
	CJellyfishFieldObj(const CJellyfishFieldObj& Obj);
	virtual ~CJellyfishFieldObj();

private:
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CRigidBody> m_Rigid;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CJellyfishFieldObj* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

