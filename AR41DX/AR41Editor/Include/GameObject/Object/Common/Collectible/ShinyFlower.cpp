#include "ShinyFlower.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CShinyFlower::CShinyFlower() :
	m_FixedFlower(false),
	m_IsMoved(false)
{
	SetTypeID<CShinyFlower>();

	m_ObjectTypeName = "ShinyFlower";

	m_ColItemType = EColItemType::ShinyFlower;
}

CShinyFlower::CShinyFlower(const CShinyFlower& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

	m_FixedFlower = Obj.m_FixedFlower;

	if (!m_FixedFlower) {
		m_ColliderPlCheckRange = (CColliderOBB3D*)FindComponent("OBB3D");
		m_IsMoved = false;
	}

	m_ColItemType = Obj.m_ColItemType;
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
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CShinyFlower>(ECollision_Result::Collision, this, &CShinyFlower::Collision_Player);

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

void CShinyFlower::Collision_Player(const CollisionResult& result)
{
	CCollectibleItems::Collision_Player(result);
}

void CShinyFlower::Collision_Range(const CollisionResult& result)
{
}
