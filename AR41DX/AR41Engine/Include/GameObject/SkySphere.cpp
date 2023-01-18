#include "SkySphere.h"
#include "../Resource/Material/Material.h"
#include "../Component/StaticMeshComponent.h"

CSkySphere::CSkySphere()
{
	SetTypeID<CSkySphere>();

	m_ObjectTypeName = "SkySphere";
}

CSkySphere::CSkySphere(const CSkySphere& Obj) : CGameObject(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CSkySphere::~CSkySphere()
{
}

void CSkySphere::Start()
{
	CGameObject::Start();
}

bool CSkySphere::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("SpherePos");
	m_Mesh->SetWorldScale(100000000.f, 100000000.f, 100000000.f);

	return true;
}

CSkySphere* CSkySphere::Clone() const
{
	return new CSkySphere(*this);
}

void CSkySphere::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CSkySphere::Load(FILE* File)
{
	CGameObject::Load(File);
}
