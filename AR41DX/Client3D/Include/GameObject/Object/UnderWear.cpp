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
