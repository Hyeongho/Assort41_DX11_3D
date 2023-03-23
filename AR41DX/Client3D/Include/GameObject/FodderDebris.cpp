#include "FodderDebris.h"
#include "Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "../../Scene/LoadingSceneInfo.h"

CFodderDebris::CFodderDebris() 
{
    SetTypeID<CFodderDebris>();

    m_ObjectTypeName = "FodderDebris";
}

CFodderDebris::CFodderDebris(const CFodderDebris& Obj)
{
    m_FodderDebrisMesh1 = (CStaticMeshComponent*)FindComponent("FodderDebrisMesh1");
    m_FodderDebrisMesh2 = (CStaticMeshComponent*)FindComponent("FodderDebrisMesh2");
    m_FodderDebrisMesh3 = (CStaticMeshComponent*)FindComponent("FodderDebrisMesh3");
    m_FodderDebrisMesh4 = (CStaticMeshComponent*)FindComponent("FodderDebrisMesh4");

    //m_Cube = (CColliderOBB3D*)FindComponent("Cube");
}

CFodderDebris::~CFodderDebris()
{
}

void CFodderDebris::Start()
{
    CGameObject::Start();
}

bool CFodderDebris::Init()
{
    CGameObject::Init();

    m_FodderDebrisMesh1 = CreateComponent<CStaticMeshComponent>("FodderDebrisMesh1");
    m_FodderDebrisMesh2 = CreateComponent<CStaticMeshComponent>("FodderDebrisMesh2");
    m_FodderDebrisMesh3 = CreateComponent<CStaticMeshComponent>("FodderDebrisMesh3");
    m_FodderDebrisMesh4 = CreateComponent<CStaticMeshComponent>("FodderDebrisMesh4");

    //m_Cube = CreateComponent<CColliderOBB3D>("Cube");

    m_Rigid = CreateComponent<CRigidBody>("Rigid");

    SetRootComponent(m_FodderDebrisMesh1);

    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh2);
    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh3);
    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh4);
    m_FodderDebrisMesh1->AddChild(m_Rigid);

    m_FodderDebrisMesh2->AddChild(m_Rigid);
    m_FodderDebrisMesh3->AddChild(m_Rigid);
    m_FodderDebrisMesh4->AddChild(m_Rigid);

    m_FodderDebrisMesh1->SetMesh("FodderDebris1");
    m_FodderDebrisMesh2->SetMesh("FodderDebris2");
    m_FodderDebrisMesh3->SetMesh("FodderDebris3");
    m_FodderDebrisMesh4->SetMesh("FodderDebris4");

    //m_FodderDebris1->SetWorldPosition(130.f, 50.f, 150.f);
    //m_ArmMesh->SetRelativePosition(0.f, 50.f, 0.f);

    m_Rigid->SetGravity(true);

    return true;
}

void CFodderDebris::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CFodderDebris::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CFodderDebris* CFodderDebris::Clone() const
{
    return new CFodderDebris(*this);
}

void CFodderDebris::Save(FILE* File)
{
}

void CFodderDebris::Load(FILE* File)
{
}

void CFodderDebris::Destroy()
{
}


