#pragma once

#include "GameObject/GameObject.h"

class CTeeterRock :
    public CGameObject
{
	friend class CScene;

protected:
	CTeeterRock();
	CTeeterRock(const CTeeterRock& Obj);
	virtual ~CTeeterRock();

private:
	CSharedPtr<class CStaticMeshComponent> m_Mesh;

	CSharedPtr<class CColliderOBB3D> m_Cube;

	bool m_IsCollison;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTeeterRock* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void CollisonPlayer(const CollisionResult& result);
	void ReleasePlayer(const CollisionResult& result);
};

