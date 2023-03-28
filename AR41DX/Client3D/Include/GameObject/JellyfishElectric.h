#pragma once

#include "GameObject\GameObject.h"

class CJellyfishElectric :
    public CGameObject
{
	friend class CScene;
	friend class CJellyfishElectric;

protected:
	CJellyfishElectric();
	CJellyfishElectric(const CJellyfishElectric& Obj);
	virtual ~CJellyfishElectric();

private:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CColliderSphere3D>	m_Collider;
	bool	m_Attack;
	float	m_AttackTime;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CJellyfishElectric* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Collision(const CollisionResult& result);
};

