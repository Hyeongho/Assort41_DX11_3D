#include "ColliderSphere3D.h"
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
#include "ColliderCube.h"
#include "ColliderOBB3D.h"
#include "../Resource/Mesh/StaticMesh.h"

CColliderSphere3D::CColliderSphere3D()
{
	SetTypeID<CColliderSphere3D>();

	m_ComponentTypeName = "ColliderSphere3D";
	m_Collider3DType = ECollider3D_Type::Sphere;

	m_Radius = 50.f;
}

CColliderSphere3D::CColliderSphere3D(const CColliderSphere3D& component) :
	CCollider3D(component)
{
	m_Radius = component.m_Radius;
}

CColliderSphere3D::~CColliderSphere3D()
{
}

void CColliderSphere3D::Start()
{
	CCollider3D::Start();
}

bool CColliderSphere3D::Init()
{
	if (!CCollider3D::Init())
		return false;


	//if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("SpherePos");
		m_DebugMaterial = CResourceManager::GetInst()->FindMaterial("DebugDecal");
	}

	//m_Mesh->GetMaterial(0)->SetEmissiveColor(1.0f, 1.0f, 1.0f, 0.f);

	return true;
}

void CColliderSphere3D::Update(float DeltaTime)
{
	CCollider3D::Update(DeltaTime);
}

void CColliderSphere3D::PostUpdate(float DeltaTime)
{
	CCollider3D::PostUpdate(DeltaTime);

	m_Info.Center = GetWorldPos();

	float Scale = GetWorldScale().x > GetWorldScale().y ? GetWorldScale().x : GetWorldScale().y;
	Scale = Scale > GetWorldScale().z ? Scale : GetWorldScale().z;

	m_Info.Radius = m_Radius * Scale;

	Vector3	Min, Max;

	Min = -m_Info.Radius;
	Max = m_Info.Radius;

	m_Transform->SetMin(Min);
	m_Transform->SetMax(Max);
}

void CColliderSphere3D::Render()
{
	CCollider3D::Render();

	m_WireFrame->SetState();

	Matrix	matScale, matTranslate, matWorld;

	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	Vector3	Scale = GetWorldScale();
	Scale.x = m_Info.Radius;
	Scale.y = m_Info.Radius;
	Scale.z = m_Info.Radius;
	/*Scale.x = m_Transform->GetRadius();
	Scale.y = m_Transform->GetRadius();
	Scale.z = m_Transform->GetRadius();*/

	matScale.Scaling(Scale);
	matTranslate.Translation(GetWorldPos());

	matWorld = matScale * matTranslate;

	CColliderConstantBuffer* Buffer = CResourceManager::GetInst()->GetColliderCBuffer();

	Buffer->SetColor(m_Color);
	Buffer->SetWVP(matWorld * matView * matProj);

	Buffer->UpdateBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();

	m_WireFrame->ResetState();
}

void CColliderSphere3D::RenderDebug()
{
	CCollider3D::RenderDebug();
}

CColliderSphere3D* CColliderSphere3D::Clone() const
{
	return new CColliderSphere3D(*this);
}

void CColliderSphere3D::Save(FILE* File)
{
	CCollider3D::Save(File);

	fwrite(&m_Radius, sizeof(float), 1, File);
}

void CColliderSphere3D::Load(FILE* File)
{
	CCollider3D::Load(File);

	fread(&m_Radius, sizeof(float), 1, File);
}

bool CColliderSphere3D::Collision(CCollider* Dest)
{
	Vector3	HitPoint;
	bool	Result = false;

	switch (((CCollider3D*)Dest)->GetCollider3DType())
	{
	case ECollider3D_Type::Box:
		Result = CCollisionManager::GetInst()->CollisionSphere3DToCube(HitPoint, this, (CColliderCube*)Dest);
		break;
	case ECollider3D_Type::OBB:
		Result = CCollisionManager::GetInst()->CollisionSphere3DToOBB3D(HitPoint, this, (CColliderOBB3D*)Dest);
		break;
	case ECollider3D_Type::Sphere:
		Result = CCollisionManager::GetInst()->CollisionSphere3DToSphere3D(HitPoint, this, (CColliderSphere3D*)Dest);
		break;
	}

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
	m_Result.HitPoint = m_HitPoint;
	Dest->SetCollisionResultHitPoint(m_HitPoint);
	return Result;
}
