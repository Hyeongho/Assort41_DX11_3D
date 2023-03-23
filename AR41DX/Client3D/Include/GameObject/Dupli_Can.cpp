#include "Dupli_Can.h"
#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Component/RigidBody.h"
#include "Scene/NavigationManager3D.h"
#include "Scene/Scene.h"
#include "Hammer.h"

CDupli_Can::CDupli_Can()	: m_FallTime(0.f)
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

	m_Mesh->SetMesh("DuplicatotronCan");

	SetRootComponent(m_Mesh);

    m_Mesh->AddChild(m_Cube);
    m_Mesh->AddChild(m_Rigid);

	m_Mesh->SetWorldPositionY(500.f);

    m_Cube->SetBoxHalfSize(50.f, 50.f, 50.f);
    m_Cube->SetRelativePosition(0.f, 0.f,0.f);

    m_Rigid->SetGravity(true);

    return true;
}

void CDupli_Can::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
	
	if (m_Rigid->GetGround()) //Ground가 아예 안 들어온다. 
	{
		m_Rigid->SetGround(false);
		/*m_Rigid->AddForce(0, 500.f, 0.f);
		m_Rigid->SetVelocityY(500.f);*/
		m_Rigid->AddForce(0, 0.f, 0.f);
		m_Rigid->SetVelocityY(0.f);
	}

	m_Rigid->SetGravityForce(20.f);
	m_Rigid->AddForce(0, 100.f, 0.f);
	m_Rigid->SetVelocityY(-100.f); // 이걸 음수로 줘야 떨어짐

	if (m_Rigid->GetVelocity().y < 0.f)
	{
		//AddWorldPositionX(100.f * DeltaTime);
		float Y = Nav->GetHeight(GetWorldPos());

		if (Y != FLT_MAX && GetWorldPos().y - Y < m_Mesh->GetMeshSize().y / 2.f)
		{
			SetWorldPositionY(Y + m_Mesh->GetMeshSize().y / 2.f);
			m_Rigid->SetGround(true);
			
			SpawnHammer();

			m_Mesh->Destroy();
			m_Mesh->ClearMaterial();
		}
	}
}

void CDupli_Can::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CDupli_Can* CDupli_Can::Clone() const
{
    return new CDupli_Can(*this);
}

void CDupli_Can::Save(FILE* File)
{
	CMonster::Save(File);
}

void CDupli_Can::Load(FILE* File)
{
	CMonster::Load(File);
}

void CDupli_Can::Collision_Ground(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		CHammer* Hammer = m_Scene->CreateObject<CHammer>("Hammer");
		Hammer->SetWorldPosition(GetWorldPos());

		return;
	}
}

void CDupli_Can::SpawnHammer()
{
	if (!m_Mesh)
	{
		return;
	}


	CHammer* Hammer = m_Scene->CreateObject<CHammer>("Hammer");
	Hammer->SetWorldPosition(GetWorldPos());
}
