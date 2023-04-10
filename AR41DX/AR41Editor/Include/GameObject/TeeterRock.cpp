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
#include "Component/RigidBody.h"
#include "Player.h"

CTeeterRock::CTeeterRock() : m_LeftCollison(false), m_RightCollison(false), m_IsColl(false), m_Time(0.f)
{
	SetTypeID<CTeeterRock>();

	m_ObjectTypeName = "TeeterRock";
}

CTeeterRock::CTeeterRock(const CTeeterRock& Obj) : CGameObject(Obj)
{
	m_Mesh = dynamic_cast<CStaticMeshComponent*>(FindComponent("Mesh"));
	m_LeftCube = dynamic_cast<CColliderOBB3D*>(FindComponent("LeftCube"));
	m_RightCube = dynamic_cast<CColliderOBB3D*>(FindComponent("RightCube"));
}

CTeeterRock::~CTeeterRock()
{
}

void CTeeterRock::Start()
{
	CGameObject::Start();

	m_LeftCube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Collision, this, &CTeeterRock::LeftCollisonPlayer);
	m_LeftCube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Release, this, &CTeeterRock::LeftReleasePlayer);

	m_RightCube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Collision, this, &CTeeterRock::RightCollisonPlayer);
	m_RightCube->SetCollisionCallback<CTeeterRock>(ECollision_Result::Release, this, &CTeeterRock::RightReleasePlayer);
}

bool CTeeterRock::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_LeftCube = CreateComponent<CColliderOBB3D>("LeftCube");
	m_RightCube = CreateComponent<CColliderOBB3D>("RightCube");

	m_LeftCube->SetCollisionProfile("Ground");	
	m_RightCube->SetCollisionProfile("Ground");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("SM_JF_Teeter_Rock_02");

	m_Mesh->SetMeshSize(m_Mesh->GetMeshSize().x * 1.5f, m_Mesh->GetMeshSize().y * 1.f, m_Mesh->GetMeshSize().z * 1.f);

	//m_Mesh->GetMaterial(0)->SetOpacity(0.f);

	m_LeftCube->AddWorldPositionX(( -1.f * m_Mesh->GetMeshSize().x / 4.f) + -1.f);
	m_RightCube->AddWorldPositionX(m_Mesh->GetMeshSize().x / 4.f);

	m_LeftCube->SetBoxHalfSize(m_Mesh->GetMeshSize() / 4.f);
	m_RightCube->SetBoxHalfSize(m_Mesh->GetMeshSize() / 4.f);
	//m_RightCube->SetBoxHalfSize(250.f, 50.f, 50.f);

	m_Mesh->AddChild(m_LeftCube);
	m_Mesh->AddChild(m_RightCube);

	m_LeftCube->SetInheritRotX(true);
	m_LeftCube->SetInheritRotY(true);
	m_LeftCube->SetInheritRotZ(true);

	m_RightCube->SetInheritRotX(true);
	m_RightCube->SetInheritRotY(true);
	m_RightCube->SetInheritRotZ(true);

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
			AddWorldRotationZ(20.f * DeltaTime);
		}

		if (m_RightCollison)
		{
			AddWorldRotationZ(-20.f * DeltaTime);
		}

		if (!m_LeftCollison && !m_RightCollison)
		{
			if (GetWorldRot().z != 0.f)
			{
				if (GetWorldRot().z > 0.f)
				{
					AddWorldRotationZ(-20.f * DeltaTime);
				}

				else if (GetWorldRot().z > 0.f)
				{
					AddWorldRotationZ(20.f * DeltaTime);
				}
			}
		}
	}
}

CTeeterRock* CTeeterRock::Clone() const
{
	return new CTeeterRock(*this);
}

void CTeeterRock::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTeeterRock::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CTeeterRock::LeftCollisonPlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		if (!m_IsColl)
		{
			m_IsColl = true;
		}

		m_LeftCollison = true;

		/*if (!m_Mesh->FindComponent("PlayerMesh"))
		{
			CSceneComponent* Component = result.Dest->GetOwner()->GetRootComponent();

			m_Mesh->AddChild(Component);

			Component->SetWorldPosition(Vector3(Component->GetWorldPos().x, Component->GetWorldPos().y - 1, Component->GetWorldPos().z));

			CPlayer* Player = dynamic_cast<CPlayer*>(m_Scene->GetPlayerObject());

			if (Player)
			{
				Player->GetRigidBody()->SetGround(true);
			}
		}*/
	}
}

void CTeeterRock::RightCollisonPlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		if (!m_IsColl)
		{
			m_IsColl = true;
		}

		m_RightCollison = true;

		/*if (!m_Mesh->FindComponent("PlayerMesh"))
		{
			CSceneComponent* Component = result.Dest->GetOwner()->GetRootComponent();

			m_Mesh->AddChild(Component);

			Component->SetWorldPosition(Vector3(Component->GetWorldPos().x, Component->GetWorldPos().y - 1, Component->GetWorldPos().z));

			CPlayer* Player = dynamic_cast<CPlayer*>(m_Scene->GetPlayerObject());

			if (Player)
			{
				Player->GetRigidBody()->SetGround(true);
			}
		}*/
	}
}

void CTeeterRock::LeftReleasePlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		if (m_IsColl)
		{
			m_IsColl = false;
			m_Time = 0.f;
		}

		m_LeftCollison = false;
	}
}

void CTeeterRock::RightReleasePlayer(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		if (m_IsColl)
		{
			m_IsColl = false;
			m_Time = 0.f;
		}

		m_RightCollison = false;
	}
}

