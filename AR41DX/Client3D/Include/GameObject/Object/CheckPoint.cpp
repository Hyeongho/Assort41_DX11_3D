#include "CheckPoint.h"
#include "../Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderCube.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CCheckPoint::CCheckPoint()
{
    SetTypeID<CCheckPoint>();

    m_ObjectTypeName = "CheckPoint";
}

CCheckPoint::CCheckPoint(const CCheckPoint& Obj)
{
    m_FlagMesh = (CAnimationMeshComponent*)FindComponent("FlagMesh");
    m_BottomMesh = (CStaticMeshComponent*)FindComponent("BottomMesh");
    m_CheckPointCube = (CColliderCube*)FindComponent("CheckPointCube");
    m_DetectRange = (CColliderCube*)FindComponent("DetectRange");
}

CCheckPoint::~CCheckPoint()
{
}

void CCheckPoint::Start()
{
    CGameObject::Start();

    m_DetectRange->SetCollisionCallback<CCheckPoint>(ECollision_Result::Collision, this, &CCheckPoint::Collision_CheckPoint_Active);
    m_DetectRange->SetCollisionCallback<CCheckPoint>(ECollision_Result::Collision, this, &CCheckPoint::Release_CheckPoint_Inactive);
}

bool CCheckPoint::Init()
{
    CGameObject::Init();

    m_FlagMesh = CreateComponent<CAnimationMeshComponent>("FlagMesh");
    m_BottomMesh = CreateComponent<CStaticMeshComponent>("BottomMesh");
    m_CheckPointCube = CreateComponent<CColliderCube>("CheckPointCube");
    m_DetectRange = CreateComponent<CColliderCube>("DetectRange");


    m_FlagMesh->SetMesh("CheckPointMesh"); 

    //m_BottomMesh->SetMesh("JumpTreeTop"); // ¼öÁ¤

    SetRootComponent(m_BottomMesh);

    m_FlagMesh->AddChild(m_CheckPointCube);
    m_FlagMesh->AddChild(m_DetectRange);

    m_BottomMesh->AddChild(m_CheckPointCube);
    m_BottomMesh->AddChild(m_DetectRange);

    m_CheckPointCube->SetCollisionProfile("Wall");
    m_CheckPointCube->SetCubeSize(300.f, 700.f, 200.f);

    m_DetectRange->SetCollisionProfile("Wall");
    m_DetectRange->SetCubeSize(500.f, 700.f, 500.f);

    m_Animation = m_FlagMesh->SetAnimation<CAnimation>("CheckPoint"); 
    m_Animation->AddAnimation("CheckPointInactive", "CheckPointInactive", 1.f, 1.f, false); 
    m_Animation->AddAnimation("CheckPointIdle", "CheckPointIdle", 1.f, 1.f, false); 
    m_Animation->AddAnimation("CheckPointPopUp", "CheckPointPopUp", 1.f, 1.f, false); 

    return true;
}

void CCheckPoint::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CCheckPoint::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CCheckPoint* CCheckPoint::Clone() const
{
    return new CCheckPoint(*this);
}

void CCheckPoint::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CCheckPoint::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CCheckPoint::Collision_CheckPoint_Active(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        //m_Animation->ChangeAnimation("JumpTreeTop_Bounce");

        //CPlayer* Player = (CPlayer*)CSceneManager::GetInst()->GetScene()->FindObject("Player");

        //CRigidBody* PlayerRigid = (CRigidBody*)Player->FindComponent("Rigid");
        //PlayerRigid->AddForce(0, 1000.f);
        //PlayerRigid->SetVelocityY(1000.f);
    }

}

void CCheckPoint::Release_CheckPoint_Inactive(const CollisionResult& result)
{
}
