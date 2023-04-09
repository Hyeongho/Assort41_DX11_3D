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

	CSharedPtr<class CColliderOBB3D> m_LeftCube;
	CSharedPtr<class CColliderOBB3D> m_RightCube;
	//CSharedPtr<class CColliderOBB3D> m_Cube;

	bool m_LeftCollison;
	bool m_RightCollison;

	bool m_IsColl;
	float m_Time;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTeeterRock* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void LeftCollisonPlayer(const CollisionResult& result);
	void RightCollisonPlayer(const CollisionResult& result);

	void LeftReleasePlayer(const CollisionResult& result);
	void RightReleasePlayer(const CollisionResult& result);
};

