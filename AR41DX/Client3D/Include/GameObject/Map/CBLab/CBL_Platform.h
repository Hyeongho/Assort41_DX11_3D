#pragma once

#include "GameObject\GameObject.h"

enum class ERollDir {
	Left,
	Right
};

class CCBL_Platform
	: public CGameObject
{
	friend class CScene;

protected:
	CCBL_Platform();
	CCBL_Platform(const CCBL_Platform& Obj);
	virtual ~CCBL_Platform();

protected:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CColliderOBB3D>		m_Collider;

private :
	bool		m_Roll;
	ERollDir	m_RollDir;
	int			m_RollCount;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCBL_Platform* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	void RoboSpongeAttackedCollision(const CollisionResult& result);


private :
	void DebugF1();
	void DebugF2();
};

