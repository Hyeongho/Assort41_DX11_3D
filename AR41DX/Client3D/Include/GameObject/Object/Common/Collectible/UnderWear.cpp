#include "UnderWear.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CUnderWear::CUnderWear()
{
}

CUnderWear::CUnderWear(const CUnderWear& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
}

CUnderWear::~CUnderWear()
{
}

void CUnderWear::Start()
{
	CCollectibleItems::Start();
}

bool CUnderWear::Init()
{
	CCollectibleItems::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Collider);
	m_Mesh->SetMesh("UnderWear");

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CUnderWear>(ECollision_Result::Collision, this, &CUnderWear::PlayerCollisionItem);

	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);

    return true;
}

void CUnderWear::Update(float DeltaTime)
{
	CCollectibleItems::Update(DeltaTime);
}

void CUnderWear::PostUpdate(float DeltaTime)
{
	CCollectibleItems::PostUpdate(DeltaTime);
}

CUnderWear* CUnderWear::Clone() const
{
    return new CUnderWear(*this);
}

void CUnderWear::Save(FILE* File)
{
	CCollectibleItems::Save(File);
}

void CUnderWear::Load(FILE* File)
{
	CCollectibleItems::Load(File);
}

void CUnderWear::PlayerCollisionItem(const CollisionResult& result)
{
	CCollectibleItems::PlayerCollisionItem(result);
}
