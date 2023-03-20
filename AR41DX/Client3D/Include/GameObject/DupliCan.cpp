#include "DupliCan.h"
#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Component/RigidBody.h"
#include "Scene/NavigationManager3D.h"
#include "Scene/Scene.h"

CDupliCan::CDupliCan()
{
	SetTypeID<CDupliCan>();

	m_ObjectTypeName = "DupliCan";
}

CDupliCan::CDupliCan(const CDupliCan& Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Cube = (CColliderOBB3D*)FindComponent("Cube");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CDupliCan::~CDupliCan()
{
}

void CDupliCan::Start()
{
	CMonster::Start();

	m_Cube->SetCollisionCallback<CDupliCan>(ECollision_Result::Collision, this, &CDupliCan::Collision_Ground);

}

bool CDupliCan::Init()
{
	CMonster::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Cube = CreateComponent<CColliderOBB3D>("Cube");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Dupli_Can");

	m_Mesh->AddChild(m_Cube);
	m_Mesh->AddChild(m_Rigid);

	m_Cube->SetBoxHalfSize(50.f, 70.f, 50.f);
	m_Cube->SetRelativePosition(0.f, 70.f, 0.f);

	m_Rigid->SetGravity(true);

	return true;
}

void CDupliCan::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	/*CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();

	if (m_Rigid->GetGround())
	{
		m_Rigid->SetGround(false);
		m_Rigid->AddForce(0, 1000.f, 0.f);
		m_Rigid->SetVelocityY(1000.f);

		float Y = Nav->GetHeight(GetWorldPos());
		if (Y != FLT_MAX)
		{
			SetWorldPositionY(Y);
		}
	}

	if (m_Rigid->GetVelocity().y < 0.f)
	{
		float Y = Nav->GetHeight(GetWorldPos());

		if (Y != FLT_MAX && GetWorldPos().y - Y < m_Mesh->GetMeshSize().y / 2.f)
		{
			SetWorldPositionY(Y + m_Mesh->GetMeshSize().y / 2.f);
			m_Rigid->SetGround(true);
		}
	}*/
}

void CDupliCan::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);

	//AddWorldPositionX(10.f * DeltaTime);
}

CDupliCan* CDupliCan::Clone() const
{
	return new CDupliCan;
}

void CDupliCan::Save(FILE* File)
{
}

void CDupliCan::Load(FILE* File)
{
}

void CDupliCan::Collision_Ground(const CollisionResult& result)
{
	/*if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Ground)
	{
		m_Mesh->SetMesh("Hammer");

		return;
	}*/
}
