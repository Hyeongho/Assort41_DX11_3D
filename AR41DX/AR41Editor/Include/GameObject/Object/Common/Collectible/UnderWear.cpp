#include "UnderWear.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CUnderWear::CUnderWear()
{
	SetTypeID<CUnderWear>();

	m_ObjectTypeName = "UnderWear";

	m_ColItemType = EColItemType::UnderWear;
}

CUnderWear::CUnderWear(const CUnderWear& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

	m_ColItemType = Obj.m_ColItemType;
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
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CUnderWear>(ECollision_Result::Collision, this, &CUnderWear::Collision_Player);

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

void CUnderWear::Collision_Player(const CollisionResult& result)
{
	CCollectibleItems::Collision_Player(result);
}
