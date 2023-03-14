#include "JumpTree.h"
#include "../Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"


CJumpTree::CJumpTree()
{
    SetTypeID<CJumpTree>();

    m_ObjectTypeName = "JumpTree";
}

CJumpTree::CJumpTree(const CJumpTree& Obj)
{
    m_TopMesh = (CAnimationMeshComponent*)FindComponent("TopMesh");
    m_BottomMesh = (CStaticMeshComponent*)FindComponent("BottomMesh");
    m_TopCube = (CColliderOBB3D*)FindComponent("TopCube");
    m_BottomCube = (CColliderOBB3D*)FindComponent("BottomCube");
}

CJumpTree::~CJumpTree()
{
}

void CJumpTree::Start()
{
    CGameObject::Start();

    m_TopCube->SetCollisionCallback<CJumpTree>(ECollision_Result::Collision, this, &CJumpTree::Collision_Bounce);
    m_TopCube->SetCollisionCallback<CJumpTree>(ECollision_Result::Release, this, &CJumpTree::Release_Bounce);

}

bool CJumpTree::Init()
{
    CGameObject::Init();

    m_TopMesh = CreateComponent<CAnimationMeshComponent>("TopMesh");
    m_BottomMesh = CreateComponent<CStaticMeshComponent>("BottomMesh");
    m_TopCube = CreateComponent<CColliderOBB3D>("TopCube");
    m_BottomCube = CreateComponent<CColliderOBB3D>("BottomCube");

    //m_TopMesh->SetMesh("JumpTreeTop");
    m_BottomMesh->SetMesh("JumpTreeBottom");

    m_TopMesh->SetMesh("JumpTreeTop");

    SetRootComponent(m_BottomMesh);

    m_BottomMesh->AddChild(m_TopMesh);
    m_BottomMesh->AddChild(m_BottomCube);

    m_TopMesh->SetRelativePositionY(900.f);

    //m_BottomCube->SetRelativePositionY(70.f);
    m_BottomCube->SetCollisionProfile("Wall");
    m_BottomCube->SetBoxHalfSize(100.f, 900.f, 100.f);

    m_TopMesh->AddChild(m_TopCube);

    //m_TopCube->SetRelativePositionY(70.f);
    m_TopCube->SetCollisionProfile("Wall");
    m_TopCube->SetBoxHalfSize(200, 100.f, 200.f);
    m_TopCube->SetRelativePositionY(100.f);

    m_Animation = m_TopMesh->SetAnimation<CAnimation>("JumpTreeTop");
    m_Animation->AddAnimation("JumpTreeTop_Idle", "JumpTreeTop_Bounce", 0.f, 0.f, false);
    m_Animation->AddAnimation("JumpTreeTop_Bounce", "JumpTreeTop_Bounce", 1.f, 1.f, false);


    return true;
}

void CJumpTree::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CJumpTree::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CJumpTree* CJumpTree::Clone() const
{
    return new CJumpTree(*this);
}

void CJumpTree::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CJumpTree::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CJumpTree::Collision_Bounce(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        m_Animation->ChangeAnimation("JumpTreeTop_Idle");

        CPlayer* Player = (CPlayer*)CSceneManager::GetInst()->GetScene()->FindObject("Player");

        CRigidBody* PlayerRigid = (CRigidBody*)Player->FindComponent("Rigid");
        PlayerRigid->AddForce(0, 1000.f);
        PlayerRigid->SetVelocityY(1000.f);
    }
    // 아래는 플레이어 기본 점프 force, velocity.
    //m_Rigid->AddForce(0, 500.f);
    //m_Rigid->SetVelocityY(500.f);
}

void CJumpTree::Release_Bounce(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        m_Animation->ChangeAnimation("JumpTreeTop_Bounce");

        CPlayer* Player = (CPlayer*)CSceneManager::GetInst()->GetScene()->FindObject("Player");

        CRigidBody* PlayerRigid = (CRigidBody*)Player->FindComponent("Rigid");

        PlayerRigid->AddForce(0, 1000.f);
        PlayerRigid->SetVelocityY(1700.f);
    }

}
