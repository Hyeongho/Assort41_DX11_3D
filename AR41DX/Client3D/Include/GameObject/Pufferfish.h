#pragma once

#include "GameObject/GameObject.h"

class CPufferfish :
    public CGameObject
{
	friend class CScene;

protected:
	CPufferfish();
	CPufferfish(const CPufferfish& Obj);
	virtual ~CPufferfish();

private:
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CColliderSphere3D> m_Sphere;
	CSharedPtr<class CAnimation> m_Animation;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPufferfish* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	void CollisionTest(const CollisionResult& result);
};

