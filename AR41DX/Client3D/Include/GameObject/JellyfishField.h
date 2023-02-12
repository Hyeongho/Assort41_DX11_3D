#pragma once

#include "GameObject\GameObject.h"

class CJellyfishField :
	public CGameObject
{
	friend class CScene;

protected:
	CJellyfishField();
	CJellyfishField(const CJellyfishField& Obj);
	virtual ~CJellyfishField();

private:
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CRigidBody> m_Rigid;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CJellyfishField* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

