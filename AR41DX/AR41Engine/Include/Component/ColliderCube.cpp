#include "ColliderCube.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Scene/CameraManager.h"
#include "../Resource/Material/Material.h"
#include "../Engine.h"
#include "../Resource/ResourceManager.h"
#include "CameraComponent.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../CollisionManager.h"
#include "ColliderOBB3D.h"
#include "ColliderSphere2D.h"
#include "ColliderPixel.h"
#include "../Resource/Mesh/StaticMesh.h"

CColliderCube::CColliderCube()
{
	SetTypeID<CColliderCube>();

	SetRenderLayerName("Collider");

	m_ComponentTypeName = "ColliderCube";
	m_Collider3DType = ECollider3D_Type::Box;
	m_CubeSize.x = 100.f;
	m_CubeSize.y = 100.f;
	m_CubeSize.z = 100.f;
}

CColliderCube::CColliderCube(const CColliderCube& component)
{
	m_CubeSize = component.m_CubeSize;
}

CColliderCube::~CColliderCube()
{
}

void CColliderCube::Start()
{
	CCollider3D::Start();
}

bool CColliderCube::Init()
{
	if (!CCollider3D::Init())
		return false;

#ifdef _DEBUG
	if (m_Scene)
	{
		m_DebugMesh = (CStaticMesh*)m_Scene->GetResource()->FindMesh("CubeLinePos");
		m_DebugMaterial = m_Scene->GetResource()->FindMaterial("DebugDecal");
	}

	else
	{
		m_DebugMesh = (CStaticMesh*)CResourceManager::GetInst()->FindMesh("CubeLinePos");
		m_DebugMaterial = CResourceManager::GetInst()->FindMaterial("DebugDecal");
	}
#endif // _DEBUG

	return true;
}

void CColliderCube::Update(float DeltaTime)
{
	CCollider3D::Update(DeltaTime);
}

void CColliderCube::PostUpdate(float DeltaTime)
{
	CCollider3D::PostUpdate(DeltaTime);

	Vector3	Size = m_CubeSize;
	Size.x *= GetWorldScale().x;
	Size.y *= GetWorldScale().y;
	Size.z *= GetWorldScale().z;

	Vector3	Min, Max;

	Min.x = GetWorldPos().x - Size.x * 0.5f;
	Min.y = GetWorldPos().y - Size.y * 0.5f;
	Min.z = GetWorldPos().z - Size.y * 0.5f;

	Max.x = Min.x + Size.x;
	Max.y = Min.y + Size.y;
	Max.z = Min.z + Size.z;

	m_Info.Left = Min.x;
	m_Info.Bottom = Min.y;
	m_Info.Right = Max.x;
	m_Info.Top = Max.y;
	m_Info.Front = Min.z;
	m_Info.Back = Max.z;

	Min.x -= GetWorldPos().x;
	Min.y -= GetWorldPos().y;
	Min.z -= GetWorldPos().z;

	Max.x -= GetWorldPos().x;
	Max.y -= GetWorldPos().y;
	Max.z -= GetWorldPos().z;

	m_Transform->SetMin(Min);
	m_Transform->SetMax(Max);
}

void CColliderCube::Render()
{
	CCollider3D::Render();

	Matrix matScale, matTranslate, matWorld;

	Matrix matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	Vector3	Scale = GetWorldScale();
	Scale.x *= m_CubeSize.x;
	Scale.y *= m_CubeSize.y;
	Scale.z *= m_CubeSize.z;

	matScale.Scaling(Scale);
	matTranslate.Translation(GetWorldPos());

	matWorld = matScale * matTranslate;

	CColliderConstantBuffer* Buffer = CResourceManager::GetInst()->GetColliderCBuffer();

	Buffer->SetColor(m_Color);
	Buffer->SetWVP(matWorld * matView * matProj);

	Buffer->UpdateBuffer();

	m_Shader->SetShader();

	m_DebugMesh->Render();
}

void CColliderCube::RenderDebug()
{
	CCollider3D::RenderDebug();
}

CColliderCube* CColliderCube::Clone() const
{
	return new CColliderCube(*this);
}

void CColliderCube::Save(FILE* File)
{
}

void CColliderCube::Load(FILE* File)
{
}

bool CColliderCube::Collision(CCollider* Dest)
{
	Vector3	HitPoint;
	bool Result = false;

	switch (((CCollider3D*)Dest)->GetCollider3DType())
	{
	case ECollider3D_Type::Box:
		Result = CCollisionManager::GetInst()->CollisionCubeToCube(HitPoint, this, (CColliderCube*)Dest);
		break;
	case ECollider3D_Type::OBB:
		Result = CCollisionManager::GetInst()->CollisionCubeToOBB3D(HitPoint, this, (CColliderOBB3D*)Dest);
		break;
	case ECollider3D_Type::Sphere:
		//Result = CCollisionManager::GetInst()->CollisionBox2DToSphere2D(HitPoint, this, (CColliderSphere2D*)Dest);
		break;

	}

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);

	return Result;
}
