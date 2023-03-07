#include "ColliderOBB3D.h"
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
#include "ColliderSphere2D.h"
#include "ColliderPixel.h"
#include "ColliderCube.h"
#include "../Resource/Mesh/StaticMesh.h"

CColliderOBB3D::CColliderOBB3D()
{
	SetTypeID<CColliderOBB3D>();

	SetRenderLayerName("Collider");

	m_ComponentTypeName = "ColliderOBB3D";
	m_Collider3DType = ECollider3D_Type::OBB;
	m_BoxHalfSize.x = 100.f;
	m_BoxHalfSize.y = 100.f;
	m_BoxHalfSize.z = 100.f;
}

CColliderOBB3D::CColliderOBB3D(const CColliderOBB3D& component) : CCollider3D(component)
{
	m_BoxHalfSize = component.m_BoxHalfSize;
}

CColliderOBB3D::~CColliderOBB3D()
{
}

void CColliderOBB3D::Start()
{
	CCollider3D::Start();
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
}

bool CColliderOBB3D::Init()
{
	if (!CCollider3D::Init())
	{
		return false;
	}
	return true;
}

void CColliderOBB3D::Update(float DeltaTime)
{
	CCollider3D::Update(DeltaTime);
}

void CColliderOBB3D::PostUpdate(float DeltaTime)
{
	CCollider3D::PostUpdate(DeltaTime);

	Vector3	Size = m_BoxHalfSize;
	Size.x *= GetWorldScale().x;
	Size.y *= GetWorldScale().y;
	Size.z *= GetWorldScale().z;

	m_Info.Center.x = GetWorldPos().x;
	m_Info.Center.y = GetWorldPos().y;
	m_Info.Center.z = GetWorldPos().z;

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vector3	Axis = GetWorldAxis((AXIS)i);

		m_Info.Axis[i] = Vector3(Axis.x, Axis.y, Axis.z);

		m_Info.Length[i] = Size[i];
	}

	Vector3	Pos[8];

	Pos[0] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[AXIS_X] + m_Info.Axis[AXIS_Y] * m_Info.Length[AXIS_Y] + m_Info.Axis[AXIS_Z] * m_Info.Length[AXIS_Z];

	Pos[1] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[AXIS_X] - m_Info.Axis[AXIS_Y] * m_Info.Length[AXIS_Y] + m_Info.Axis[AXIS_Z] * m_Info.Length[AXIS_Z];

	Pos[2] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[AXIS_X] + m_Info.Axis[AXIS_Y] * m_Info.Length[AXIS_Y] - m_Info.Axis[AXIS_Z] * m_Info.Length[AXIS_Z];

	Pos[3] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[AXIS_X] - m_Info.Axis[AXIS_Y] * m_Info.Length[AXIS_Y] + m_Info.Axis[AXIS_Z] * m_Info.Length[AXIS_Z];

	Pos[4] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[AXIS_X] + m_Info.Axis[AXIS_Y] * m_Info.Length[AXIS_Y] - m_Info.Axis[AXIS_Z] * m_Info.Length[AXIS_Z];

	Pos[5] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[AXIS_X] - m_Info.Axis[AXIS_Y] * m_Info.Length[AXIS_Y] - m_Info.Axis[AXIS_Z] * m_Info.Length[AXIS_Z];

	Pos[6] = m_Info.Center + m_Info.Axis[AXIS_X] * m_Info.Length[AXIS_X] + m_Info.Axis[AXIS_Y] * m_Info.Length[AXIS_Y] + m_Info.Axis[AXIS_Z] * m_Info.Length[AXIS_Z];

	Pos[7] = m_Info.Center - m_Info.Axis[AXIS_X] * m_Info.Length[AXIS_X] - m_Info.Axis[AXIS_Y] * m_Info.Length[AXIS_Y] - m_Info.Axis[AXIS_Z] * m_Info.Length[AXIS_Z];


	Vector3	Min, Max;

	Min.x = Pos[0].x;
	Min.y = Pos[0].y;
	Min.z = Pos[0].z;

	Max.x = Pos[0].x;
	Max.y = Pos[0].y;
	Max.z = Pos[0].z;

	for (int i = 1; i < 8; ++i)
	{
		Min.x = Min.x > Pos[i].x ? Pos[i].x : Min.x;
		Min.y = Min.y > Pos[i].y ? Pos[i].y : Min.y;
		Min.z = Min.z > Pos[i].z ? Pos[i].z : Min.z;

		Max.x = Max.x < Pos[i].x ? Pos[i].x : Max.x;
		Max.y = Max.y < Pos[i].y ? Pos[i].y : Max.y;
		Max.z = Max.z < Pos[i].z ? Pos[i].z : Max.z;
	}

	Min.x -= GetWorldPos().x;
	Min.y -= GetWorldPos().y;
	Min.z -= GetWorldPos().z;

	Max.x -= GetWorldPos().x;
	Max.y -= GetWorldPos().y;
	Max.z -= GetWorldPos().z;

	m_Transform->SetMin(Min);
	m_Transform->SetMax(Max);
}

void CColliderOBB3D::Render()
{
	CCollider3D::Render();
}

void CColliderOBB3D::RenderDebug()
{
	CCollider3D::RenderDebug();

	Matrix	matScale, matRot, matTranslate, matWorld;

	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	Vector3	Scale = Vector3(m_Info.Length[0] * 2.f, m_Info.Length[1] * 2.f, m_Info.Length[2] * 2.f);

	matScale.Scaling(Scale);
	matRot.Rotation(GetWorldRot());
	matTranslate.Translation(GetWorldPos());

	matWorld = matScale * matRot * matTranslate;

	CColliderConstantBuffer* Buffer = CResourceManager::GetInst()->GetColliderCBuffer();

	Buffer->SetColor(m_Color);
	Buffer->SetWVP(matWorld * matView * matProj);

	Buffer->UpdateBuffer();

	m_Shader->SetShader();

	m_DebugMesh->Render();
}

CColliderOBB3D* CColliderOBB3D::Clone() const
{
	return new CColliderOBB3D(*this);
}

void CColliderOBB3D::Save(FILE* File)
{
	CCollider3D::Save(File);

	fwrite(&m_BoxHalfSize, sizeof(Vector3), 1, File);
}

void CColliderOBB3D::Load(FILE* File)
{
	CCollider3D::Load(File);

	fread(&m_BoxHalfSize, sizeof(Vector3), 1, File);
}

bool CColliderOBB3D::Collision(CCollider* Dest)
{
	Vector3	HitPoint;
	bool Result = false;

	switch (((CCollider3D*)Dest)->GetCollider3DType())
	{
	case ECollider3D_Type::Box:
		Result = CCollisionManager::GetInst()->CollisionCubeToOBB3D(HitPoint, (CColliderCube*)Dest, this);
		break;
	case ECollider3D_Type::OBB:
		Result = CCollisionManager::GetInst()->CollisionOBB3DToOBB3D(HitPoint, this, (CColliderOBB3D*)Dest);
		break;
	case ECollider3D_Type::Sphere:
		//Result = CCollisionManager::GetInst()->CollisionBox2DToSphere2D(HitPoint, this, (CColliderSphere2D*)Dest);
		break;

	}

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);

	return Result;
}

