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

CTeeterRock::CTeeterRock() : m_LeftCollison(false), m_RightCollison(false)
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
	m_LeftCube = CreateComponent<CColliderOBB3D>("Cube");
	m_RightCube = CreateComponent<CColliderOBB3D>("m_RightCube");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("SM_JF_Teeter_Rock_01");

	m_LeftCube->AddWorldPositionX(-251.f);
	m_RightCube->AddWorldPositionX(251.f);

	m_LeftCube->SetBoxHalfSize(250.f, 50.f, 50.f);
	m_RightCube->SetBoxHalfSize(250.f, 50.f, 50.f);

	m_Mesh->AddChild(m_LeftCube);
	m_Mesh->AddChild(m_RightCube);

	m_LeftCube->SetInheritRotX(true);
	m_LeftCube->SetInheritRotY(true);
	m_LeftCube->SetInheritRotZ(true);

	m_RightCube->SetInheritRotX(true);
	m_RightCube->SetInheritRotY(true);
	m_RightCube->SetInheritRotZ(true);

	m_LeftCube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Collision, this, &CTeeterRock::LeftCollisonPlayer);
	m_LeftCube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Release, this, &CTeeterRock::LeftReleasePlayer);

	m_RightCube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Collision, this, &CTeeterRock::RightCollisonPlayer);
	m_RightCube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Release, this, &CTeeterRock::RightReleasePlayer);

	AddWorldRotationY(90.f);

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

	if (Object)
	{
		if (m_LeftCollison)
		{
			AddWorldRotationZ(10.f * DeltaTime);
		}

		if (m_RightCollison)
		{
			AddWorldRotationZ(-10.f * DeltaTime);
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

void CTeeterRock::LeftCollisonPlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		m_LeftCollison = true;
	}
}

void CTeeterRock::RightCollisonPlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		m_RightCollison = true;
	}
}

void CTeeterRock::LeftReleasePlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		m_LeftCollison = false;
	}
}

void CTeeterRock::RightReleasePlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		m_RightCollison = false;
	}
}

