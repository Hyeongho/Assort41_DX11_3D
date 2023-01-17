#include "Collider2D.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Engine.h"

CCollider2D::CCollider2D()
{
	SetTypeID<CCollider2D>();

	m_ComponentTypeName = "Collider2D";
	m_ColliderType = ECollider_Type::Collider2D;
}

CCollider2D::CCollider2D(const CCollider2D& component) :
	CCollider(component)
{
	m_Collider2DType = component.m_Collider2DType;
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::Start()
{
	CCollider::Start();
}

bool CCollider2D::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CCollider2D::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CCollider2D::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);
}

void CCollider2D::Render()
{
	CCollider::Render();
}

void CCollider2D::Save(FILE* File)
{
	CCollider::Save(File);

	fwrite(&m_Collider2DType, sizeof(ECollider2D_Type), 1, File);
}

void CCollider2D::Load(FILE* File)
{
	CCollider::Load(File);

	fread(&m_Collider2DType, sizeof(ECollider2D_Type), 1, File);
}
