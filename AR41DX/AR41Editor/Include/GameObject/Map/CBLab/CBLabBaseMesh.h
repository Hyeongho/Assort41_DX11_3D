#pragma once

#include "GameObject\GameObject.h"

class CCBLabBaseMesh :
	public CGameObject
{
	friend class CScene;

protected:
	CCBLabBaseMesh();
	CCBLabBaseMesh(const CCBLabBaseMesh& Obj);
	virtual ~CCBLabBaseMesh();

private:
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CRigidBody> m_Rigid;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCBLabBaseMesh* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

