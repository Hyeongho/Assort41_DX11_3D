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
#include "ColliderCube.h"
#include "ColliderSphere2D.h"
#include "ColliderPixel.h"

CColliderOBB3D::CColliderOBB3D()
{
	SetTypeID<CColliderOBB3D>();

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
}

bool CColliderOBB3D::Init()
{
	if (!CCollider3D::Init())
	{
		return false;
	}


	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box3DLineMesh");
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

	for (int i = 0; i < AXIS2D_MAX; ++i)
	{
		Vector3	Axis = GetWorldAxis((AXIS)i);

		m_Info.Axis[i] = Vector3(Axis.x, Axis.y, Axis.z);

		m_Info.Length[i] = Size[i];
	}

	Vector3	Pos[8];

	//-++
	//+-+
	//++-
	//--+
	//-+-
	//+--
	//+++
	//---

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
}

CColliderOBB3D* CColliderOBB3D::Clone() const
{
	return new CColliderOBB3D(*this);
}

void CColliderOBB3D::Save(FILE* File)
{
}

void CColliderOBB3D::Load(FILE* File)
{
}

bool CColliderOBB3D::Collision(CCollider* Dest)
{
	return true;
}

