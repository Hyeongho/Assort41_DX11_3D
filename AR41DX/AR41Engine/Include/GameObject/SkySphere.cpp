#include "SkySphere.h"
#include "../Resource/Material/Material.h"
#include "../Component/StaticMeshComponent.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"

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

void CSkySphere::SetSkyTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Material->SetTexture(0, 0, (int)EShaderBufferType::Pixel, Name, FileName, PathName);
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

	if (m_Scene)
	{
		m_Material = m_Scene->GetResource()->FindMaterial("Sky");
	}

	else
	{
		m_Material = CResourceManager::GetInst()->FindMaterial("Sky");
	}

	m_Mesh->AddMaterial(m_Material);

	m_Material = m_Mesh->GetMaterial(0);

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
