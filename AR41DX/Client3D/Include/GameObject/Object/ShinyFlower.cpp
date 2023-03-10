#include "ShinyFlower.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CShinyFlower::CShinyFlower()
{
}

CShinyFlower::CShinyFlower(const CShinyFlower& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
}

CShinyFlower::~CShinyFlower()
{
}

void CShinyFlower::Start()
{
	CCollectibleItems::Start();
}

bool CShinyFlower::Init()
{
	CCollectibleItems::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Collider);
	m_Mesh->SetMesh("ShinyFlower");

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CShinyFlower>(ECollision_Result::Collision, this, &CShinyFlower::PlayerCollisionItem);

	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);

	return true;
}

void CShinyFlower::Update(float DeltaTime)
{
	CCollectibleItems::Update(DeltaTime);
}

void CShinyFlower::PostUpdate(float DeltaTime)
{
	CCollectibleItems::PostUpdate(DeltaTime);
}

CShinyFlower* CShinyFlower::Clone() const
{
	return new CShinyFlower(*this);
}

void CShinyFlower::Save(FILE* File)
{
	CCollectibleItems::Save(File);
}

void CShinyFlower::Load(FILE* File)
{
	CCollectibleItems::Load(File);
}

void CShinyFlower::PlayerCollisionItem(const CollisionResult& result)
{
	CCollectibleItems::PlayerCollisionItem(result);
}
