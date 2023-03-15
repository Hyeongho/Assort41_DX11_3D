#pragma once
#include "GameObject/GameObject.h"

class CIceCube : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CColliderOBB3D> m_Body;
	void CollisionIn(const CollisionResult& result);
	void CollisionOut(const CollisionResult& result);
protected:
	CIceCube();
	CIceCube(const CIceCube& obj);
	virtual ~CIceCube();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CIceCube* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

