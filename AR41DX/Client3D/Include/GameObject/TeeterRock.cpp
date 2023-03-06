#include "TeeterRock.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"

CTeeterRock::CTeeterRock() : m_IsCollison(false)
{
}

CTeeterRock::CTeeterRock(const CTeeterRock& Obj)
{
}

CTeeterRock::~CTeeterRock()
{
}

void CTeeterRock::Start()
{
	CGameObject::Start();
}

bool CTeeterRock::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Cube = CreateComponent<CColliderOBB3D>("Cube");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Cube);

	m_Cube->SetBoxHalfSize(500.f, 50.f, 50.f);

	m_Cube->SetInheritRotX(true);
	m_Cube->SetInheritRotY(true);
	m_Cube->SetInheritRotZ(true);

	m_Cube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Collision, this, &CTeeterRock::CollisonPlayer);
	m_Cube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Release, this, &CTeeterRock::ReleasePlayer);

	return true;
}

void CTeeterRock::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTeeterRock::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	CGameObject* Object = m_Scene->GetPlayerObject();

	if (Object && m_IsCollison)
	{
		if (Object->GetWorldPos().x >= GetWorldPos().x)
		{
			AddWorldRotationZ(-10.0f * DeltaTime);
		}

		else
		{
			AddWorldRotationZ(10.0f * DeltaTime);
		}
	}
}

CTeeterRock* CTeeterRock::Clone() const
{
	return new CTeeterRock(*this);
}

void CTeeterRock::Save(FILE* File)
{
}

void CTeeterRock::Load(FILE* File)
{
}

void CTeeterRock::CollisonPlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		m_IsCollison = true;
	}
}

void CTeeterRock::ReleasePlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		m_IsCollison = false;
	}
}
