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

CColliderCube::CColliderCube()
{
	SetTypeID<CColliderCube>();

	m_ComponentTypeName = "ColliderCube";
	m_Collider3DType = ECollider3D_Type::Box;
	m_CubeSize.x = 100.f;
	m_CubeSize.y = 100.f;
	m_CubeSize.z = 100.f;
}

CColliderCube::CColliderCube(const CColliderCube& component)
{
}

CColliderCube::~CColliderCube()
{
}

void CColliderCube::Start()
{
}

bool CColliderCube::Init()
{
	return true;
}

void CColliderCube::Update(float DeltaTime)
{
}

void CColliderCube::PostUpdate(float DeltaTime)
{
}

void CColliderCube::Render()
{
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
	return true;
}
