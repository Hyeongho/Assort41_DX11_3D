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
