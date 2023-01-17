#include "ColliderOBB2D.h"
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
#include "ColliderBox2D.h"
#include "ColliderSphere2D.h"
#include "ColliderPixel.h"

CColliderOBB2D::CColliderOBB2D()
{
	SetTypeID<CColliderOBB2D>();

	m_ComponentTypeName = "ColliderOBB2D";
	m_Collider2DType = ECollider2D_Type::OBB2D;

	m_BoxHalfSize.x = 50.f;
	m_BoxHalfSize.y = 50.f;
}

CColliderOBB2D::CColliderOBB2D(const CColliderOBB2D& component) :
	CCollider2D(component)
{
	m_BoxHalfSize = component.m_BoxHalfSize;
}

CColliderOBB2D::~CColliderOBB2D()
{
}

void CColliderOBB2D::Start()
{
	CCollider2D::Start();
}

bool CColliderOBB2D::Init()
{
	if (!CCollider2D::Init())
		return false;


	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box2DLineMesh");
	}

	return true;
}

void CColliderOBB2D::Update(float DeltaTime)
{
	CCollider2D::Update(DeltaTime);
}

void CColliderOBB2D::PostUpdate(float DeltaTime)
{
	CCollider2D::PostUpdate(DeltaTime);

	Vector2	Size = m_BoxHalfSize;
	Size.x *= GetWorldScale().x;
	Size.y *= GetWorldScale().y;

	m_Info.Center.x = GetWorldPos().x;
	m_Info.Center.y = GetWorldPos().y;

	for (int i = 0; i < AXIS2D_MAX; ++i)
	{
		Vector3	Axis = GetWorldAxis((AXIS)i);

		m_Info.Axis[i] = Vector2(Axis.x, Axis.y);

		m_Info.Length[i] = Size[i];
	}


	Vector2	Pos[4];

	Pos[0] = m_Info.Center - m_Info.Axis[AXIS2D_X] * m_Info.Length[AXIS2D_X] +
		m_Info.Axis[AXIS2D_Y] * m_Info.Length[AXIS2D_Y];

	Pos[1] = m_Info.Center + m_Info.Axis[AXIS2D_X] * m_Info.Length[AXIS2D_X] +	
		m_Info.Axis[AXIS2D_Y] * m_Info.Length[AXIS2D_Y];

	Pos[2] = m_Info.Center - m_Info.Axis[AXIS2D_X] * m_Info.Length[AXIS2D_X] -
		m_Info.Axis[AXIS2D_Y] * m_Info.Length[AXIS2D_Y];

	Pos[3] = m_Info.Center + m_Info.Axis[AXIS2D_X] * m_Info.Length[AXIS2D_X] -
		m_Info.Axis[AXIS2D_Y] * m_Info.Length[AXIS2D_Y];

	m_Min.x = Pos[0].x;
	m_Min.y = Pos[0].y;

	m_Max.x = Pos[0].x;
	m_Max.y = Pos[0].y;

	for (int i = 1; i < 4; ++i)
	{
		m_Min.x = m_Min.x > Pos[i].x ? Pos[i].x : m_Min.x;
		m_Min.y = m_Min.y > Pos[i].y ? Pos[i].y : m_Min.y;

		m_Max.x = m_Max.x < Pos[i].x ? Pos[i].x : m_Max.x;
		m_Max.y = m_Max.y < Pos[i].y ? Pos[i].y : m_Max.y;
	}
}

void CColliderOBB2D::Render()
{
	CCollider2D::Render();

	Matrix	matScale, matRot, matTranslate, matWorld;

	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	Vector3	Scale = Vector3(m_Info.Length[0] * 2.f, m_Info.Length[1] * 2.f, 1.f);

	matScale.Scaling(Scale);
	matRot.Rotation(GetWorldRot());
	matTranslate.Translation(GetWorldPos());

	matWorld = matScale * matRot * matTranslate;

	CColliderConstantBuffer* Buffer = CResourceManager::GetInst()->GetColliderCBuffer();

	Buffer->SetColor(m_Color);
	Buffer->SetWVP(matWorld * matView * matProj);

	Buffer->UpdateBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();
}

CColliderOBB2D* CColliderOBB2D::Clone() const
{
	return new CColliderOBB2D(*this);
}

void CColliderOBB2D::Save(FILE* File)
{
	CCollider2D::Save(File);

	fwrite(&m_BoxHalfSize, sizeof(Vector2), 1, File);
}

void CColliderOBB2D::Load(FILE* File)
{
	CCollider2D::Load(File);

	fread(&m_BoxHalfSize, sizeof(Vector2), 1, File);

	if (CEngine::GetEditorMode())
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box2DLineMesh");
	}
}

bool CColliderOBB2D::Collision(CCollider* Dest)
{
	Vector2	HitPoint;
	bool	Result = false;

	switch (((CCollider2D*)Dest)->GetCollider2DType())
	{
	case ECollider2D_Type::Box2D:
		Result = CCollisionManager::GetInst()->CollisionBox2DToOBB2D(HitPoint, (CColliderBox2D*)Dest, this);
		break;
	case ECollider2D_Type::OBB2D:
		Result = CCollisionManager::GetInst()->CollisionOBB2DToOBB2D(HitPoint, this, (CColliderOBB2D*)Dest);
		break;
	case ECollider2D_Type::Sphere2D:
		Result = CCollisionManager::GetInst()->CollisionSphere2DToOBB2D(HitPoint, (CColliderSphere2D*)Dest, this);
		break;
	case ECollider2D_Type::Pixel:
		Result = CCollisionManager::GetInst()->CollisionOBB2DToPixel(HitPoint, this, (CColliderPixel*)Dest);
		break;
	}

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);

	return Result;
}

bool CColliderOBB2D::CollisionMouse(const Vector2& MouseWorldPos)
{
	Vector2	HitPoint;

	m_MouseCollision = CCollisionManager::GetInst()->CollisionPointToOBB2D(HitPoint, MouseWorldPos,
		m_Info);

	m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);

	return m_MouseCollision;
}
