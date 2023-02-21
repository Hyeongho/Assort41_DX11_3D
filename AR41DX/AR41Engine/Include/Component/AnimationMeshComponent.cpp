
#include "AnimationMeshComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CAnimationMeshComponent::CAnimationMeshComponent()
{
	SetTypeID<CAnimationMeshComponent>();

	m_ComponentTypeName = "AnimationMeshComponent";
}

CAnimationMeshComponent::CAnimationMeshComponent(const CAnimationMeshComponent& component) :
	CPrimitiveComponent(component)
{
	if (component.m_Skeleton)
	{
		m_Skeleton = component.m_Skeleton->Clone();
	}
	m_Socket = nullptr;

	if (component.m_Animation)
	{
		m_Animation = component.m_Animation->Clone();

		if (m_Skeleton)
			m_Animation->SetSkeleton(m_Skeleton);

		if (m_Mesh)
			m_Animation->SetInstancingBoneBuffer(((CAnimationMesh*)m_Mesh.Get())->GetBoneBuffer());
	}
}

CAnimationMeshComponent::~CAnimationMeshComponent()
{
}

void CAnimationMeshComponent::GetAnimationNames(std::vector<std::string>& vecNames)
{
	if (m_Animation)
		m_Animation->GetAnimationNames(vecNames);
}

void CAnimationMeshComponent::SetAnimation(CAnimation* anim)
{
	m_Animation = anim;
}

bool CAnimationMeshComponent::SetMesh(const std::string& Name)
{
	CPrimitiveComponent::SetMesh(Name);

	CAnimationMesh* Mesh = (CAnimationMesh*)m_Mesh.Get();

	m_Skeleton = Mesh->GetSkeleton()->Clone();

	if (m_Animation)
	{
		m_Animation->SetSkeleton(m_Skeleton);
		m_Animation->SetInstancingBoneBuffer(Mesh->GetBoneBuffer());
	}

	return true;
}

bool CAnimationMeshComponent::SetMesh(CMesh* Mesh)
{
	CPrimitiveComponent::SetMesh(Mesh);

	CAnimationMesh* AnimMesh = (CAnimationMesh*)m_Mesh.Get();

	m_Skeleton = AnimMesh->GetSkeleton()->Clone();

	if (m_Animation)
	{
		m_Animation->SetSkeleton(m_Skeleton);
		m_Animation->SetInstancingBoneBuffer(AnimMesh->GetBoneBuffer());
	}

	return true;
}

bool CAnimationMeshComponent::SetMesh(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
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

	CAnimationMesh* AnimMesh = (CAnimationMesh*)m_Mesh.Get();

	m_Skeleton = AnimMesh->GetSkeleton()->Clone();

	if (m_Animation)
	{
		m_Animation->SetSkeleton(m_Skeleton);
		m_Animation->SetInstancingBoneBuffer(AnimMesh->GetBoneBuffer());
	}

	return true;
}

bool CAnimationMeshComponent::SetMeshFullPath(const std::string& Name,
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

	CAnimationMesh* AnimMesh = (CAnimationMesh*)m_Mesh.Get();

	m_Skeleton = AnimMesh->GetSkeleton()->Clone();

	if (m_Animation)
	{
		m_Animation->SetSkeleton(m_Skeleton);
		m_Animation->SetInstancingBoneBuffer(AnimMesh->GetBoneBuffer());
	}

	return true;
}

void CAnimationMeshComponent::Start()
{
	CPrimitiveComponent::Start();

	if (m_Animation)
		m_Animation->Start();
}

bool CAnimationMeshComponent::Init()
{
	if (!CPrimitiveComponent::Init())
		return false;

	return true;
}

void CAnimationMeshComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	if (m_Animation)
		m_Animation->Update(DeltaTime);
}

void CAnimationMeshComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CAnimationMeshComponent::Render()
{
	if (m_Animation)
		m_Animation->SetShader();

	CPrimitiveComponent::Render();

	if (m_Animation)
		m_Animation->ResetShader();
}

CAnimationMeshComponent* CAnimationMeshComponent::Clone() const
{
	return new CAnimationMeshComponent(*this);
}

void CAnimationMeshComponent::Save(FILE* File)
{
	CPrimitiveComponent::Save(File);
}

void CAnimationMeshComponent::Load(FILE* File)
{
	CPrimitiveComponent::Load(File);
}
