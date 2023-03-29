#include "Dupli_Can.h"
#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Component/RigidBody.h"
#include "Scene/NavigationManager3D.h"
#include "Scene/Scene.h"

CDupli_Can::CDupli_Can()
{
	SetTypeID<CDupli_Can>();

	m_ObjectTypeName = "Dupli_Can";
}

CDupli_Can::CDupli_Can(const CDupli_Can& Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Cube = (CColliderOBB3D*)FindComponent("Cube");
	m_Rigid = (CRigidBody*)FindComponent("Rigid"); 
}

CDupli_Can::~CDupli_Can()
{
}

void CDupli_Can::Start()
{
	CMonster::Start();

    m_Cube->SetCollisionCallback<CDupli_Can>(ECollision_Result::Collision, this, &CDupli_Can::Collision_Ground);

}

bool CDupli_Can::Init()
{
	CMonster::Init();

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
    m_Cube = CreateComponent<CColliderOBB3D>("Cube");
    m_Rigid = CreateComponent<CRigidBody>("Rigid");

    SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("DuplicatotronCan");

    m_Mesh->AddChild(m_Cube);
    m_Mesh->AddChild(m_Rigid);

    m_Cube->SetBoxHalfSize(50.f, 70.f, 50.f);
    m_Cube->SetRelativePosition(0.f, 70.f,0.f);

    m_Rigid->SetGravity(true);

    return true;
}

void CDupli_Can::Update(float DeltaTime)
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

void CDupli_Can::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);

	//AddWorldPositionX(10.f * DeltaTime);
}

CDupli_Can* CDupli_Can::Clone() const
{
    return new CDupli_Can;
}

void CDupli_Can::Save(FILE* File)
{
}

void CDupli_Can::Load(FILE* File)
{
}

void CDupli_Can::Collision_Ground(const CollisionResult& result)
{
	/*if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Ground)
	{
		m_Mesh->SetMesh("Hammer");

		return;
	}*/
}
