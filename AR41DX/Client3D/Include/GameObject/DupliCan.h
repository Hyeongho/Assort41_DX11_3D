
#pragma once
#include "Monster.h"

class CDupliCan :
    public CMonster
{
	friend class CScene;

protected:
	CDupliCan();
	CDupliCan(const CDupliCan& Obj);
	virtual ~CDupliCan();

private:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CColliderOBB3D>	m_Cube;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CDupliCan* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

		void Collision_Ground(const CollisionResult& result);
};

