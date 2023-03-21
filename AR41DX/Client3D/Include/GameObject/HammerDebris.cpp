#include "HammerDebris.h"
#include "Player.h"
#include "Hammer.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "../../Scene/LoadingSceneInfo.h"
#include "Scene/NavigationManager3D.h"
#include "Component/RigidBody.h"

CHammerDebris::CHammerDebris()
{
    SetTypeID<CHammerDebris>();

    m_ObjectTypeName = "HammerDebris";
}

CHammerDebris::CHammerDebris(const CHammerDebris& Obj)
{
    m_HammerDebrisMesh1 = (CStaticMeshComponent*)FindComponent("HammerDebrisMesh1");
    m_HammerDebrisMesh2 = (CStaticMeshComponent*)FindComponent("HammerDebrisMesh2");
    m_HammerDebrisMesh3 = (CStaticMeshComponent*)FindComponent("HammerDebrisMesh3");
    m_HammerDebrisMesh4 = (CStaticMeshComponent*)FindComponent("HammerDebrisMesh4");

    //m_Cube = (CColliderOBB3D*)FindComponent("Cube");
}

CHammerDebris::~CHammerDebris()
{
}

void CHammerDebris::Start()
{
    CGameObject::Start();
}

bool CHammerDebris::Init()
{
    CGameObject::Init();

    m_HammerDebrisMesh1 = CreateComponent<CStaticMeshComponent>("HammerDebrisMesh1");
    m_HammerDebrisMesh2 = CreateComponent<CStaticMeshComponent>("HammerDebrisMesh2");
    m_HammerDebrisMesh3 = CreateComponent<CStaticMeshComponent>("HammerDebrisMesh3");
    m_HammerDebrisMesh4 = CreateComponent<CStaticMeshComponent>("HammerDebrisMesh4");

    //m_Cube = CreateComponent<CColliderOBB3D>("Cube");

    m_Rigid = CreateComponent<CRigidBody>("Rigid");

    SetRootComponent(m_HammerDebrisMesh1);

    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh2);
    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh3);
    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh4);
    m_HammerDebrisMesh1->AddChild(m_Rigid);

    m_HammerDebrisMesh1->SetMesh("HammerDebris1");

    m_HammerDebrisMesh2->SetMesh("HammerDebris2");
    m_HammerDebrisMesh2->SetRelativePosition(0.f, 50.f, 150.f);
    m_HammerDebrisMesh2->AddChild(m_Rigid);

    m_HammerDebrisMesh3->SetMesh("HammerDebris3");
    m_HammerDebrisMesh3->SetRelativePosition(0.f, 50.f, 160.f);
    m_HammerDebrisMesh3->AddChild(m_Rigid);

    m_HammerDebrisMesh4->SetMesh("HammerDebris4");
    m_HammerDebrisMesh4->SetRelativePosition(0.f, 50.f, 50.f);
    m_HammerDebrisMesh4->AddChild(m_Rigid);

    m_HammerDebrisMesh1->SetWorldPositionY(200.f);

  


    //m_HammerDebris1->SetWorldPosition(130.f, 50.f, 150.f);
    //m_ArmMesh->SetRelativePosition(0.f, 50.f, 0.f);

    m_Rigid->SetGravity(true);

    return true;
}

void CHammerDebris::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    CHammer* Hammer = (CHammer*)CSceneManager::GetInst()->GetScene()->FindObject("Hammer");
    
    if (Hammer->GetDebrisOn() == true)
    {
        CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();

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

            if (Y != FLT_MAX && GetWorldPos().y - Y < m_HammerDebrisMesh1->GetMeshSize().y / 2.f)
            {
                SetWorldPositionY(Y + m_HammerDebrisMesh1->GetMeshSize().y / 2.f);
                m_Rigid->SetGround(true);
            }
        }
    }

}

void CHammerDebris::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    CHammer* Hammer = (CHammer*)CSceneManager::GetInst()->GetScene()->FindObject("Hammer");

    if(Hammer->GetDebrisOn() == true)
    AddWorldPositionX(100.f * DeltaTime);
}

CHammerDebris* CHammerDebris::Clone() const
{
    return new CHammerDebris(*this);
}

void CHammerDebris::Save(FILE* File)
{
}

void CHammerDebris::Load(FILE* File)
{
}

void CHammerDebris::Destroy()
{
}


