#include "CollisionSection3D.h"
#include "../Component/Collider3D.h"

CCollisionSection3D::CCollisionSection3D()
{
}

CCollisionSection3D::~CCollisionSection3D()
{
}

void CCollisionSection3D::AddCollider(CCollider3D* Collider)
{
	m_ColliderList.push_back(Collider);

	Collider->AddSectionIndex(m_Index);
}

void CCollisionSection3D::Collision(float DeltaTime)
{
}
