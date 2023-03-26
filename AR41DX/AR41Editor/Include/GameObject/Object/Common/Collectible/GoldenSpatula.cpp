#include "GoldenSpatula.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "../../../Player.h"

CGoldenSpatula::CGoldenSpatula()
{
	SetTypeID<CGoldenSpatula>();

	m_ObjectTypeName = "GoldenSpatula";

	m_ColItemType = EItemList::GoldenSpatula;
}

CGoldenSpatula::CGoldenSpatula(const CGoldenSpatula& Obj) :
	CCollectibleItems(Obj)
{
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
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CGoldenSpatula>(ECollision_Result::Collision, this, &CGoldenSpatula::Collision_Player);


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