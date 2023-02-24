#include "Collider3D.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Engine.h"

CCollider3D::CCollider3D()
{
	SetTypeID<CCollider3D>();

	m_ComponentTypeName = "Collider3D";
	m_ColliderType = ECollider_Type::Collider3D;
}

CCollider3D::CCollider3D(const CCollider3D& component) :
	CCollider(component)
{
	m_Collider3DType = component.m_Collider3DType;
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::Start()
{
	CCollider::Start();
}

bool CCollider3D::Init()
{
	if (!CCollider::Init())
	{
		return false;
	}

	m_WireFrame = CRenderManager::GetInst()->FindRenderState<CRenderState>("WireFrame");

	return true;
}

void CCollider3D::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CCollider3D::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);
}

void CCollider3D::Render()
{
	CCollider::Render();
}

void CCollider3D::Save(FILE* File)
{
	CCollider::Save(File);

	fwrite(&m_Collider3DType, sizeof(ECollider3D_Type), 1, File);
}

void CCollider3D::Load(FILE* File)
{
	CCollider::Load(File);

	fread(&m_Collider3DType, sizeof(ECollider3D_Type), 1, File);
}


