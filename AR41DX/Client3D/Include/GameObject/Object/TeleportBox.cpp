#include "TeleportBox.h"
#include "../Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderCube.h"
#include "Scene/Scene.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "TeleportBox.h"


CTeleportBox::CTeleportBox()
{
    SetTypeID<CTeleportBox>();

    m_ObjectTypeName = "TeleportBox";
}

CTeleportBox::CTeleportBox(const CTeleportBox& Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");/*
    m_BoxWallCube1 = (CColliderCube*)FindComponent("BoxWallCube1");
    m_BoxWallCube2 = (CColliderCube*)FindComponent("BoxWallCube2");
    m_BoxWallCube3 = (CColliderCube*)FindComponent("BoxWallCube3");
    m_BoxWallCube4 = (CColliderCube*)FindComponent("BoxWallCube4");
    m_BoxBottomCube = (CColliderCube*)FindComponent("BoxBottomCube");*/
    m_DetectRange = (CColliderCube*)FindComponent("DetectRange");
}

CTeleportBox::~CTeleportBox()
{
}

void CTeleportBox::Start()
{
    CGameObject::Start();

    m_DetectRange->SetCollisionCallback<CTeleportBox>(ECollision_Result::Collision, this, &CTeleportBox::Collision_OpenBox);
}

bool CTeleportBox::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");/*
    m_BoxWallCube1 = CreateComponent<CColliderCube>("BoxWallCube1");
    m_BoxWallCube2 = CreateComponent<CColliderCube>("BoxWallCube2");
    m_BoxWallCube3 = CreateComponent<CColliderCube>("BoxWallCube3");
    m_BoxWallCube4 = CreateComponent<CColliderCube>("BoxWallCube4");
    m_BoxBottomCube = CreateComponent<CColliderCube>("BoxBottomCube");*/
    m_DetectRange = CreateComponent<CColliderCube>("DetectRange");

    m_Mesh->SetMesh("TeleportBox");

    //m_Mesh->AddChild(m_BoxWallCube1);
    //m_Mesh->AddChild(m_BoxWallCube2);
    //m_Mesh->AddChild(m_BoxWallCube3);
    //m_Mesh->AddChild(m_BoxWallCube3);
    //m_Mesh->AddChild(m_BoxBottomCube);
    m_Mesh->AddChild(m_DetectRange);

    SetRootComponent(m_Mesh);

    //// Back
    //m_BoxWallCube1->SetCollisionProfile("Wall");
    //m_BoxWallCube1->SetCubeSize(30.f, 30.f, 10.f);
    //m_BoxWallCube1->AddRelativeRotationY(90.f);
    //m_BoxWallCube1->SetRelativePosition(0.f, 0.f, -30.f);

    //// Front
    //m_BoxWallCube2->SetCollisionProfile("Wall");
    //m_BoxWallCube2->SetCubeSize(30.f, 30.f, 10.f);
    //m_BoxWallCube2->AddRelativeRotationY(90.f);
    //m_BoxWallCube2->SetRelativePosition(0.f, 0.f, 30.f);

    //// Left
    //m_BoxWallCube3->SetCollisionProfile("Wall");
    //m_BoxWallCube3->SetCubeSize(30.f, 30.f, 10.f);
    //m_BoxWallCube3->AddRelativeRotationX(90.f);
    //m_BoxWallCube3->SetRelativePosition(0.f, 0.f, -30.f);

    //// Right
    //m_BoxWallCube4->SetCollisionProfile("Wall");
    //m_BoxWallCube4->SetCubeSize(30.f, 30.f, 10.f);
    //m_BoxWallCube4->AddRelativeRotationX(90.f);
    //m_BoxWallCube4->SetRelativePosition(0.f, 0.f, -30.f);

    //// Bottom
    //m_BoxBottomCube->SetCollisionProfile("Wall");
    //m_BoxBottomCube->SetCubeSize(30.f, 30.f, 10.f);

    // DetectRange
    m_DetectRange->SetCollisionProfile("Wall");
    m_DetectRange->SetCubeSize(700.f, 10.f, 700.f);

    m_Animation = m_Mesh->SetAnimation<CAnimation>("TeleportBoxMesh");

    m_Animation->AddAnimation("TeleportBox_Closed", "TeleportBox_Closed", 1.f, 1.f, false);
    m_Animation->AddAnimation("TeleportBox_Opening", "TeleportBox_Opening", 1.f, 1.f, false);
    m_Animation->AddAnimation("TeleportBox_OpeningLoop", "TeleportBox_OpeningLoop", 1.f, 1.f, false);

    return true;
}

void CTeleportBox::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CTeleportBox::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CTeleportBox* CTeleportBox::Clone() const
{
    return new CTeleportBox(*this);
}

void CTeleportBox::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CTeleportBox::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CTeleportBox::Collision_OpenBox(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        m_Animation->ChangeAnimation("TeleportBox_Opening");
    }
}
