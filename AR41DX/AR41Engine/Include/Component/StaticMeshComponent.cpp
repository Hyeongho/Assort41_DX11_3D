
#include "StaticMeshComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	SetTypeID<CStaticMeshComponent>();

	m_ComponentTypeName = "StaticMeshComponent";
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& component) :
	CPrimitiveComponent(component)
{
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

bool CStaticMeshComponent::SetMesh(const std::string& Name)
{
	return CPrimitiveComponent::SetMesh(Name);
}

bool CStaticMeshComponent::SetMesh(CMesh* Mesh)
{
	return CPrimitiveComponent::SetMesh(Mesh);
}

bool CStaticMeshComponent::SetMesh(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (m_Scene)
	{
		m_Scene->GetResource()->LoadMesh(MeshType::Static, Name, FileName, PathName);
		m_Mesh = m_Scene->GetResource()->FindMesh(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadMesh(m_Scene, MeshType::Static, Name, FileName, PathName);
		m_Mesh = CResourceManager::GetInst()->FindMesh(Name);
	}

	if (m_Mesh)
		SetMeshSize(m_Mesh->GetMeshSize());

	m_vecMaterial.clear();

	int SlotCount = m_Mesh->GetSlotCount();

	for (int i = 0; i < SlotCount; ++i)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		m_vecMaterial.push_back(Material->Clone());
	}

	return true;
}

bool CStaticMeshComponent::SetMeshFullPath(const std::string& Name, 
	const TCHAR* FullPath)
{
	if (m_Scene)
	{
		m_Scene->GetResource()->LoadMeshFullPath(MeshType::Static, Name, FullPath);
		m_Mesh = m_Scene->GetResource()->FindMesh(Name);
	}

	else
	{
		CResourceManager::GetInst()->LoadMeshFullPath(m_Scene, MeshType::Static, Name, FullPath);
		m_Mesh = CResourceManager::GetInst()->FindMesh(Name);
	}

	if (m_Mesh)
		SetMeshSize(m_Mesh->GetMeshSize());

	m_vecMaterial.clear();

	int SlotCount = m_Mesh->GetSlotCount();

	for (int i = 0; i < SlotCount; ++i)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		m_vecMaterial.push_back(Material->Clone());
	}

	return true;
}

void CStaticMeshComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CStaticMeshComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}

	if (m_Scene)
	{
		m_ShadowMapShader = (CGraphicShader*)m_Scene->GetResource()->FindShader("ShadowMapStaticShader");
	}

	else
	{
		m_ShadowMapShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("ShadowMapStaticShader");
	}


	//김범중 에디터에서 빈오브젝트에서 생성했을때 에러 방지용 기본 메쉬
	SetMesh("GizMo");
	SetWorldScale(10.f, 10.f, 10.f);
	return true;
}

void CStaticMeshComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);
}

void CStaticMeshComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CStaticMeshComponent::Render()
{
	CPrimitiveComponent::Render();
}

void CStaticMeshComponent::RenderShadowMap()
{
	CPrimitiveComponent::RenderShadowMap();
}

CStaticMeshComponent* CStaticMeshComponent::Clone() const
{
	return new CStaticMeshComponent(*this);
}

void CStaticMeshComponent::Save(FILE* File)
{
	CPrimitiveComponent::Save(File);
}

void CStaticMeshComponent::Load(FILE* File)
{
	CPrimitiveComponent::Load(File);
}
