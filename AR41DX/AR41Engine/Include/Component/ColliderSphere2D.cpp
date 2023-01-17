#include "ColliderSphere2D.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Engine.h"
#include "../Resource/ResourceManager.h"
#include "CameraComponent.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../CollisionManager.h"
#include "ColliderBox2D.h"
#include "ColliderOBB2D.h"
#include "ColliderPixel.h"

CColliderSphere2D::CColliderSphere2D()
{
	SetTypeID<CColliderSphere2D>();

	m_ComponentTypeName = "ColliderSphere2D";
	m_Collider2DType = ECollider2D_Type::Sphere2D;

	m_Radius = 50.f;
}

CColliderSphere2D::CColliderSphere2D(const CColliderSphere2D& component) :
	CCollider2D(component)
{
	m_Radius = component.m_Radius;
}

CColliderSphere2D::~CColliderSphere2D()
{
}

void CColliderSphere2D::Start()
{
	CCollider2D::Start();
}

bool CColliderSphere2D::Init()
{
	if (!CCollider2D::Init())
		return false;


	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Sphere2DLineMesh");
	}

	return true;
}

void CColliderSphere2D::Update(float DeltaTime)
{
	CCollider2D::Update(DeltaTime);
}

void CColliderSphere2D::PostUpdate(float DeltaTime)
{
	CCollider2D::PostUpdate(DeltaTime);

	m_Info.Center.x = GetWorldPos().x;
	m_Info.Center.y = GetWorldPos().y;

	float Scale = GetWorldScale().x > GetWorldScale().y ? GetWorldScale().x : GetWorldScale().y;

	m_Info.Radius = m_Radius * Scale;

	m_Min.x = GetWorldPos().x - m_Info.Radius;
	m_Min.y = GetWorldPos().y - m_Info.Radius;

	m_Max.x = m_Min.x + m_Info.Radius * 2.f;
	m_Max.y = m_Min.y + m_Info.Radius * 2.f;
}

void CColliderSphere2D::Render()
{
	CCollider2D::Render();

	Matrix	matScale, matTranslate, matWorld;

	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	Vector3	Scale = GetWorldScale();
	Scale.x = m_Info.Radius;
	Scale.y = m_Info.Radius;

	matScale.Scaling(Scale);
	matTranslate.Translation(GetWorldPos());

	matWorld = matScale * matTranslate;

	CColliderConstantBuffer* Buffer = CResourceManager::GetInst()->GetColliderCBuffer();

	Buffer->SetColor(m_Color);
	Buffer->SetWVP(matWorld * matView * matProj);

	Buffer->UpdateBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();
}

CColliderSphere2D* CColliderSphere2D::Clone() const
{
	return new CColliderSphere2D(*this);
}

void CColliderSphere2D::Save(FILE* File)
{
	CCollider2D::Save(File);

	fwrite(&m_Radius, sizeof(float), 1, File);
}

void CColliderSphere2D::Load(FILE* File)
{
	CCollider2D::Load(File);

	fread(&m_Radius, sizeof(float), 1, File);
}

bool CColliderSphere2D::Collision(CCollider* Dest)
{
	Vector2	HitPoint;
	bool	Result = false;

	switch (((CCollider2D*)Dest)->GetCollider2DType())
	{
	case ECollider2D_Type::Box2D:
		Result = CCollisionManager::GetInst()->CollisionBox2DToSphere2D(HitPoint, (CColliderBox2D*)Dest, this);
		break;
	case ECollider2D_Type::OBB2D:
		Result = CCollisionManager::GetInst()->CollisionSphere2DToOBB2D(HitPoint, this, (CColliderOBB2D*)Dest);
		break;
	case ECollider2D_Type::Sphere2D:
		Result = CCollisionManager::GetInst()->CollisionSphere2DToSphere2D(HitPoint, this, (CColliderSphere2D*)Dest);
		break;
	case ECollider2D_Type::Pixel:
		Result = CCollisionManager::GetInst()->CollisionSphere2DToPixel(HitPoint, this, (CColliderPixel*)Dest);
		break;
	}

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);

	return Result;
}

bool CColliderSphere2D::CollisionMouse(const Vector2& MouseWorldPos)
{
	Vector2	HitPoint;

	m_MouseCollision = CCollisionManager::GetInst()->CollisionPointToSphere2D(HitPoint, MouseWorldPos,
		m_Info);

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);

	return m_MouseCollision;
}
