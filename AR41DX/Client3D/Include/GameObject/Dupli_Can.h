#pragma once

#include "Monster.h"

class CDupli_Can :
    public CMonster
{
	friend class CScene;

protected:
	CDupli_Can();
	CDupli_Can(const CDupli_Can& Obj);
	virtual ~CDupli_Can();

private:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CColliderOBB3D>	m_Cube;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CDupli_Can* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	void Collision_Ground(const CollisionResult& result);
};

