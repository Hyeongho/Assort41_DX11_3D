#include "Sock.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "../../../Player.h"

CSock::CSock()
{
	SetTypeID<CSock>();

	m_ObjectTypeName = "Sock";

	m_ColItemType = EItemList::Sock;
}

CSock::CSock(const CSock& Obj) :
	CCollectibleItems(Obj)
{
}

CSock::~CSock()
{
}

void CSock::Start()
{
	CCollectibleItems::Start();

	m_Collider->SetCollisionCallback<CSock>(ECollision_Result::Collision, this, &CSock::Collision_Player);


	m_Mesh->AddChild(m_Bubble);
	float ySize = m_Mesh->GetMeshSize().y;
	m_Bubble->SetRelativePositionY(ySize / 2.f);
	m_Bubble->SetInheritRotX(true);
	m_Bubble->SetInheritRotY(true);
	m_Bubble->SetInheritRotZ(true);
}

bool CSock::Init()
{
	CCollectibleItems::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Collider);
	m_Mesh->SetMesh("Sock");

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Collectible");

	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);

	
	return true;
}

void CSock::Update(float DeltaTime)
{
	CCollectibleItems::Update(DeltaTime);
}

void CSock::PostUpdate(float DeltaTime)
{
	CCollectibleItems::PostUpdate(DeltaTime);
}

CSock* CSock::Clone() const
{
	return new CSock(*this);
}

void CSock::Save(FILE* File)
{
	CCollectibleItems::Save(File);
}

void CSock::Load(FILE* File)
{
	CCollectibleItems::Load(File);
}
