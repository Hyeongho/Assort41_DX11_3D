#include "Gate.h"
#include "../Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderCube.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CGate::CGate()  :
    m_OpenSesameOn(false)
{
    SetTypeID<CGate>();

    m_ObjectTypeName = "Gate";
}

CGate::CGate(const CGate& Obj)
    : CGameObject(Obj)
{
    m_ArmMesh = (CStaticMeshComponent*)FindComponent("ArmMesh");
    m_BottomMesh = (CStaticMeshComponent*)FindComponent("BottomMesh");
    m_ArmCube = (CColliderCube*)FindComponent("m_ArmCube");
    m_BottomCube = (CColliderCube*)FindComponent("BottomCube");
    m_DetectRange = (CColliderCube*)FindComponent("m_DetectRange");
}

CGate::~CGate()
{
}

void CGate::Start()
{
    CGameObject::Start();

    m_DetectRange->SetCollisionCallback<CGate>(ECollision_Result::Collision, this, &CGate::Collision_OpenSesame);
}

bool CGate::Init()
{
    m_ArmMesh = CreateComponent<CStaticMeshComponent>("ArmMesh");
    m_BottomMesh = CreateComponent<CStaticMeshComponent>("BottomMesh");
    m_ArmCube = CreateComponent<CColliderCube>("ArmCube");
    m_BottomCube = CreateComponent<CColliderCube>("BottomCube");
    m_DetectRange = CreateComponent<CColliderCube>("DetectRange");

    m_BottomMesh->SetMesh("GateBottom"); 

    m_ArmMesh->SetMesh("GateArm"); 

    m_BottomMesh->AddChild(m_ArmMesh);
    m_BottomMesh->AddChild(m_BottomCube);
    m_BottomMesh->AddChild(m_DetectRange);

    m_ArmMesh->AddChild(m_ArmCube);

    SetRootComponent(m_BottomMesh);


    m_ArmCube->SetCollisionProfile("Wall");
    m_ArmCube->SetCubeSize(500.f, 60.f, 10.f);

    m_BottomCube->SetCollisionProfile("Wall");
    m_BottomCube->SetCubeSize(60.f, 70.f, 40.f);

    m_DetectRange->SetCollisionProfile("Wall");
    m_DetectRange->SetCubeSize(150.f, 10.f, 150.f);

    return true;
}

void CGate::Update(float DeltaTime)
{
    if(m_OpenSesameOn)
        m_ArmMesh->AddRelativeRotationY(10.f);
}

void CGate::PostUpdate(float DeltaTime)
{
}

CGate* CGate::Clone() const
{
    return nullptr;
}

void CGate::Save(FILE* File)
{
}

void CGate::Load(FILE* File)
{
}

void CGate::Collision_OpenSesame(const CollisionResult& result)
{
    m_OpenSesameOn = true;
}
