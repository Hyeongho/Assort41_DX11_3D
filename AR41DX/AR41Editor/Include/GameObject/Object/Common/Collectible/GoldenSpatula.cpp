#include "GoldenSpatula.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CGoldenSpatula::CGoldenSpatula()
{
}

CGoldenSpatula::CGoldenSpatula(const CGoldenSpatula& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
}

CGoldenSpatula::~CGoldenSpatula()
{
}

void CGoldenSpatula::Start()
{
	CCollectibleItems::Start();
}

bool CGoldenSpatula::Init()
{
	CCollectibleItems::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Collider);
	m_Mesh->SetMesh("GoldenSpatula");

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CGoldenSpatula>(ECollision_Result::Collision, this, &CGoldenSpatula::PlayerCollisionItem);

	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);


	return true;
}

void CGoldenSpatula::Update(float DeltaTime)
{
	CCollectibleItems::Update(DeltaTime);
}

void CGoldenSpatula::PostUpdate(float DeltaTime)
{
	CCollectibleItems::PostUpdate(DeltaTime);
}

CGoldenSpatula* CGoldenSpatula::Clone() const
{
	return new CGoldenSpatula(*this);
}

void CGoldenSpatula::Save(FILE* File)
{
	CCollectibleItems::Save(File);
}

void CGoldenSpatula::Load(FILE* File)
{
	CCollectibleItems::Load(File);
}

void CGoldenSpatula::PlayerCollisionItem(const CollisionResult& result)
{
	CCollectibleItems::PlayerCollisionItem(result);
}
