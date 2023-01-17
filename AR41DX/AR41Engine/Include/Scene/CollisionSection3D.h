#pragma once

#include "../EngineInfo.h"

class CCollisionSection3D
{
	friend class CSceneCollision;

private:
	CCollisionSection3D();
	~CCollisionSection3D();

private:
	Vector3		m_Min;
	Vector3		m_Max;
	int			m_IndexX;
	int			m_IndexY;
	int			m_IndexZ;
	int			m_Index;
	std::list<CSharedPtr<class CCollider3D>>	m_ColliderList;

public:
	void AddCollider(class CCollider3D* Collider);
	void Collision(float DeltaTime);
};

