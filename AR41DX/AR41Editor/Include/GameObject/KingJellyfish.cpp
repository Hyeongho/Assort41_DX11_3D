#include "KingJellyfish.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Resource/ResourceManager.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/ColliderOBB3D.h"
#include "Player.h"
#include "ElectricRing.h"
#include "Jellyfish.h"

CKingJellyfish::CKingJellyfish()
    : m_AttackTime(0.f)
    , m_Attack(false)
    , m_MoveSpeed(100.f)
    , m_CollisionCount(0)
    , m_Collision(false)
    , m_Angry(false)
    , m_Idle(true)
    , m_SpawnJellyfish(false)
    , m_OnGround(false)
    , m_Electric(false)
    , m_Damage(false)
    , m_Pool(false)
{
    SetTypeID<CKingJellyfish>();

    m_ObjectTypeName = "KingJellyfish";
}

CKingJellyfish::CKingJellyfish(const CKingJellyfish& Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("KingJellyfishMesh");
    m_EffectMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Cube = (CColliderOBB3D*)FindComponent("Cube");
}

CKingJellyfish::~CKingJellyfish()
{
}

void CKingJellyfish::Start()
{
    CGameObject::Start();
}

bool CKingJellyfish::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("KingJellyfishMesh");
    //m_EffectMesh = CreateComponent<CAnimationMeshComponent>("Mesh");
    m_Cube = CreateComponent<CColliderOBB3D>("Cube");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("KingJellyfish");
    //m_EffectMesh->SetMesh("KingJellyfish");

    m_Mesh->AddChild(m_Cube);

    m_Cube->SetBoxHalfSize(200.f, 200.f, 200.f);

    m_Cube->SetRelativePosition(50.f, 400.f);
    //m_Cube->SetEnable(true);

    m_Cube->SetCollisionProfile("Monster");

    m_Cube->SetCollisionCallback<CKingJellyfish>(ECollision_Result::Collision, this, &CKingJellyfish::Collision);


    //std::string& Name = m_Mesh->GetSkeleton()->GetBone();

    //m_EffectMesh->GetSkeleton()->

    //m_EffectMesh->SetMaterial(0, "EffectTexture");
   // m_EffectMesh->SetMaterial(0, "DefaultTexture");

   // m_Mesh->SetWorldPosition(500.f, 500.f, 0.f);

    //m_Mesh->AddChild(m_EffectMesh);
    //m_Mesh->AddChild(m_Cube);


    //m_EffectMesh->SetRelativeScale(1.25f, 1.25f);

    m_Animation = m_Mesh->SetAnimation<CAnimation>("KingJellyfishAnimation");

    m_Animation->AddAnimation("KingJellyfish_Idle", "KingJellyfish_Idle", 1.f, 0.5f, true);
    m_Animation->AddAnimation("KingJellyfish_Angry", "KingJellyfish_Angry", 1.f, 0.5f, true);
    m_Animation->AddAnimation("KingJellyfish_Attack", "KingJellyfish_Attack", 1.f, 0.5f, false);
    m_Animation->AddAnimation("KingJellyfish_Damage", "KingJellyfish_Damage", 1.f, 0.5f, true);
    m_Animation->AddAnimation("KingJellyfish_OnGround", "KingJellyfish_OnGround", 1.f, 0.5, true);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, true);

    //m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Attack", this, &CKingJellyfish::OnGround);
    //m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Damage", this, &CKingJellyfish::Angry);


    //CSkeleton* Skeleton = m_Mesh->GetSkeleton();

    //m_EffectMesh->GetAnimation()->SetSkeleton(Skeleton);

   // m_BT = new CKingJellyfishBT;

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Idle");
    CResourceManager::GetInst()->SetVolume(2.f);

    return true;
}

void CKingJellyfish::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    //m_Animation->ChangeAnimation("KingJellyfish_Idle");

    ////m_EffectMeshAnimation->ChangeAnimation("KingJellyfish_Idle");

    //m_EffectMesh->AddRelativePosition(m_Mesh->GetWorldPos());

    m_MonsterPos = m_Mesh->GetWorldPos();

    //float MeshMaxY = m_Mesh->GetMesh()->GetMax().y;

    //m_EffectMesh->SetMax(Vector3(m_EffectMesh.Get()->GetMax().x, MeshMaxY, m_EffectMesh.Get()->GetMax().z));

    //float MeshMinY = m_Mesh->GetMesh()->GetMin().y;

    //m_EffectMesh->SetMin(Vector3(m_EffectMesh.Get()->GetMin().x, MeshMaxY, m_EffectMesh.Get()->GetMin().z));

    m_PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

    m_Motion = EKingJellyfish::Attack;


    // m_BT->Run();

     //if (m_Idle)
     //{
     //    m_AttackTime += DeltaTime;

     //    m_Motion = EKingJellyfish::Idle;

     //    if (m_AttackTime > 30.f)
     //    {
     //        m_Idle = false;

     //        m_AttackTime = 0.f;

     //        m_OnGround = true;
     //    }
     //}

     //if (m_OnGround)
     //{
     //    m_Motion = EKingJellyfish::Attack;

     //    //Attack();

     //    m_Attack = true;

     //    m_OnGround = false;
     //}

     //if (m_Attack)
     //{
     //    m_AttackTime += DeltaTime / DeltaTime;

     //    m_Motion = EKingJellyfish::OnGround;

     //    //OnGround();

     //    if (m_AttackTime > 4.f)
     //    {
     //       // m_Electric = true;
     //        //m_Idle = true;

     //        m_AttackTime = 0.f;

     //        m_Attack = false;
     //    }
     //}

     //if (!m_Idle && !m_Attack && !m_OnGround)
     //    m_Motion = EKingJellyfish::Idle;

     //if (m_Electric)
     //{
     //    m_AttackTime += DeltaTime / DeltaTime;

     //    m_Motion = EKingJellyfish::OnGround;

     //    if (!m_ElectricRing)
     //    {
     //        m_ElectricRing = m_Scene->CreateObject<CElectricRing>("ElectricRing");
     //    }

     //    if (m_AttackTime > 2.f)
     //    {
     //        CResourceManager::GetInst()->SoundPlay("KingJellyfish_Charge");

     //        m_AttackTime = 0.f;

     //        m_Electric = false;

     //        //m_Idle = true;
     //    }

     //    /* if (m_Collision)
     //     {
     //         m_AttackTime = 0.f;

     //         m_Electric = false;

     //         m_Damage = true;
     //     }*/

     //}

     /*if (!m_OnGround && !m_Attack && !m_Electric)
     {
         m_Motion = EKingJellyfish::Idle;
     }*/

}

void CKingJellyfish::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    //if (m_Idle)
    //{
    //    m_AttackTime += DeltaTime;

    //    m_Motion = EKingJellyfish::Idle;

    //    if (m_AttackTime > 30.f)
    //    {
    //        m_Idle = false;

    //        m_AttackTime = 0.f;

    //        m_OnGround = true;
    //    }
    //}

    //if (m_OnGround)
    //{
    //    m_Motion = EKingJellyfish::Attack;

    //    //Attack();

    //    m_Attack = true;

    //    m_OnGround = false;
    //}

    //if (m_Attack)
    //{
    //    m_AttackTime += DeltaTime / DeltaTime;

    //    m_Motion = EKingJellyfish::OnGround;

    //    //OnGround();

    //    if (m_AttackTime > 4.f)
    //    {
    //        // m_Electric = true;
    //         //m_Idle = true;

    //        m_AttackTime = 0.f;

    //        m_Attack = false;
    //    }
    //}


    //if (m_Idle)
    //{
    //    m_AttackTime += DeltaTime / DeltaTime;

    //    EKingJellyfish::Idle;

    //    if (m_AttackTime > 10.f)
    //    {
    //        m_Idle = false;

    //        m_AttackTime = 0.f;

    //        m_OnGround = true;
    //    }
    //}

    //if (m_OnGround)
    //{
    //    EKingJellyfish::Attack;

    //    //Attack();

    //    m_Attack = true;

    //    m_OnGround = false;
    //}

    //if (m_Attack)
    //{
    //    m_AttackTime += DeltaTime / DeltaTime;

    //    EKingJellyfish::OnGround;

    //    //OnGround();

    //    if (m_AttackTime > 2.f)
    //    {
    //        m_Electric = true;

    //        m_AttackTime = 0.f;

    //        m_Attack = false;
    //    }
    //}

    //if (m_Electric)
    //{
    //    m_AttackTime += DeltaTime / DeltaTime;

    //    EKingJellyfish::OnGround;

    //   if (!m_ElectricRing)
    //   {
    //       m_ElectricRing = m_Scene->CreateObject<CElectricRing>("ElectricRing");
    //   }

    //   if (m_AttackTime > 2.f)
    //   {
    //       CResourceManager::GetInst()->SoundPlay("KingJellyfish_Charge");

    //       m_AttackTime = 0.f;

    //       m_Electric = false;

    //       m_Idle = true;
    //   }

    //  /* if (m_Collision)
    //   {
    //       m_AttackTime = 0.f;

    //       m_Electric = false;

    //       m_Damage = true;
    //   }*/
    //   
    //}

    /*
    if (m_Damage)
    {
        Damage();

        m_Angry = true;

        m_Damage = false;
    }

    if (m_Angry)
    {
        Angry();

        if (m_MonsterPos == m_ResetPos)
        {
            m_Angry = false;

            SpawnJellyfish();
        }
    }

    if (m_SpawnJellyfish)
    {
        for (int i = 0; i < 6; ++i)
        {
            m_AttackTime += DeltaTime / DeltaTime;

            if (m_AttackTime > 1.5f)
            {
               m_Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");

               m_Jellyfish->AddWorldRotationX(90.f * i);

               m_AttackTime = 0.f;
            }
        }
        m_SpawnJellyfish = false;

        m_Idle = true;
    }*/

    //switch (m_Motion)
    //{
    //case EKingJellyfish::Idle:
    //    Idle();
    //    break;
    //case EKingJellyfish::Damage:
    //    Damage();
    //    break;
    //case EKingJellyfish::Angry:
    //    Angry();
    //    break;
    //case EKingJellyfish::OnGround:
    //    OnGround();
    //    break;
    //case EKingJellyfish::SpawnJellyfish:
    //    SpawnJellyfish();
    //    break;
    //case EKingJellyfish::Attack:
    //    Attack();
    //    break;
    //}

    /*  if (!m_Angry && !m_Attack && !m_Damage && !m_OnGround && !m_Pool)
          EKingJellyfish::Idle;*/

}

CKingJellyfish* CKingJellyfish::Clone() const
{
    return new CKingJellyfish(*this);
}

void CKingJellyfish::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CKingJellyfish::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CKingJellyfish::Collision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::PlayerAttack)
    {
        m_Collision = true;

        m_CollisionCount++;

        if (m_CollisionCount == 3)
        {
            m_Mesh->Destroy();
        }
    }

    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Pool)
    {
        m_Pool = true;
    }
}

void CKingJellyfish::DetectCollision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Pool)
    {
        m_Animation->ChangeAnimation("KingJellyfish_Idle");

        Vector3 Dir = m_PlayerPos - m_MonsterPos;

        Dir.y = 0.f;

        Dir.Normalize();

        m_Mesh->AddWorldPosition(Dir * g_DeltaTime * 100.f);

    }

    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Pool)
    {
        Idle();
    }
}

void CKingJellyfish::Idle()
{

    m_Idle = true;

    m_Animation->ChangeAnimation("KingJellyfish_Idle");

    Vector3 Dir = m_PlayerPos - m_MonsterPos;

    Dir.y = 0.f;

    Dir.Normalize();

    m_Mesh->AddWorldPosition(Dir * g_DeltaTime * 100.f);
}

void CKingJellyfish::Damage()
{
    m_Animation->ChangeAnimation("KingJellyfish_Damage");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Damage");
}

void CKingJellyfish::Angry()
{
    m_Angry = true;

    m_Animation->ChangeAnimation("KingJellyfish_Angry");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Angry");

    //if (m_Pool)
    //{
    //    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Angry", this, &CKingJellyfish::SpawnJellyfish);
    //}
}

void CKingJellyfish::Attack()
{
    m_Animation->ChangeAnimation("KingJellyfish_Attack");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_OnGround");

    //m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Attack", this, &CKingJellyfish::OnGround);
}

void CKingJellyfish::OnGround()
{
    m_Animation->ChangeAnimation("KingJellyfish_OnGround");

    // m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_OnGround", this, &CKingJellyfish::Attack);
}

void CKingJellyfish::SpawnJellyfish()
{
    m_SpawnJellyfish = true;

    m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_SpawnJellyfish");
}
