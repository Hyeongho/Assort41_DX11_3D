#include "TerrainComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CTerrainComponent::CTerrainComponent() : m_CountX(0), m_CountY(0)
{
	SetTypeID<CTerrainComponent>();

	m_ComponentTypeName = "TerrainComponent";
}

CTerrainComponent::CTerrainComponent(const CTerrainComponent& component) : CPrimitiveComponent(component)
{
	m_CountX = component.m_CountX;
	m_CountY = component.m_CountY;

	m_Size = component.m_Size;
	m_CellSize = component.m_CellSize;
}

CTerrainComponent::~CTerrainComponent()
{
}

bool CTerrainComponent::SetMesh(const std::string& Name)
{
	return CPrimitiveComponent::SetMesh(Name);
}

bool CTerrainComponent::SetMesh(CMesh* Mesh)
{
	return CPrimitiveComponent::SetMesh(Mesh);
}

bool CTerrainComponent::SetMesh(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
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
	{
		SetMeshSize(m_Mesh->GetMeshSize());
	}

	m_vecMaterial.clear();

	int SlotCount = m_Mesh->GetSlotCount();

	for (int i = 0; i < SlotCount; i++)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		m_vecMaterial.push_back(Material->Clone());
	}

	return true;
}

bool CTerrainComponent::SetMeshFullPath(const std::string& Name, const TCHAR* FullPath)
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
	{
		SetMeshSize(m_Mesh->GetMeshSize());
	}

	m_vecMaterial.clear();

	int SlotCount = m_Mesh->GetSlotCount();

	for (int i = 0; i < SlotCount; i++)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		m_vecMaterial.push_back(Material->Clone());
	}

	return true;
}

void CTerrainComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CTerrainComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}

	return true;
}

void CTerrainComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);
}

void CTerrainComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CTerrainComponent::Render()
{
	CPrimitiveComponent::Render();
}

CTerrainComponent* CTerrainComponent::Clone() const
{
	return new CTerrainComponent(*this);
}

void CTerrainComponent::Save(FILE* File)
{
	CPrimitiveComponent::Save(File);
}

void CTerrainComponent::Load(FILE* File)
{
	CPrimitiveComponent::Load(File);
}

void CTerrainComponent::CreateTerrain(int CountX, int CountY, float SizeX, float SizeY, const char* HeightMapName, 
	const std::string& HeightMapPath)
{
	m_CountX = CountX;
	m_CountY = CountY;

	m_CellSize.x = SizeX;
	m_CellSize.y = SizeY;

	m_Size = m_CellSize * Vector2((float)CountX, (float)CountY);
}
