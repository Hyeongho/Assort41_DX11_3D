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
    , m_MoveSpeed(0.f)
    , m_CollisionCount(0)
    , m_Collision(false)
    , m_Angry(false)
    , m_Idle(true)
    , m_SpawnJellyfish(false)
    , m_OnGround(false)
    , m_Electric(false)
    , m_Damage(false)
    , m_Pool(false)
    , m_ReSpawn(false)
{
    SetTypeID<CKingJellyfish>();

    m_ObjectTypeName = "KingJellyfish";
}

CKingJellyfish::CKingJellyfish(const CKingJellyfish& Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("KingJellyfishMesh");
    m_EffectMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_AttackCollision = (CColliderOBB3D*)FindComponent("AttackCollision");
    m_DetectCollision = (CColliderOBB3D*)FindComponent("DetectCollision");
    m_PoolCollision = (CColliderOBB3D*)FindComponent("PoolCollision");
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
    m_AttackCollision = CreateComponent<CColliderOBB3D>("AttackCollision");
    m_DetectCollision = CreateComponent<CColliderOBB3D>("DetectCollision");
    m_PoolCollision = CreateComponent<CColliderOBB3D>("PoolCollision");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("KingJellyfish");
    //m_EffectMesh->SetMesh("KingJellyfish");

    m_Mesh->AddChild(m_AttackCollision);
    m_Mesh->AddChild(m_DetectCollision);
    m_Mesh->AddChild(m_PoolCollision);

    m_AttackCollision->SetBoxHalfSize(200.f, 200.f, 200.f);
    m_AttackCollision->SetRelativePosition(50.f, 400.f);
    //m_AttackCollision->SetEnable(true);

    m_DetectCollision->SetBoxHalfSize(700.f, 500.f, 700.f);
    m_DetectCollision->SetRelativePosition(0.f, 500.f);

    m_PoolCollision->SetBoxHalfSize(200.f, 200.f, 200.f);
    m_PoolCollision->SetRelativePosition(0.f, 200.f);

    m_AttackCollision->SetCollisionProfile("Monster");
    m_DetectCollision->SetCollisionProfile("Monster");
    m_PoolCollision->SetCollisionProfile("Monster");

    m_AttackCollision->SetCollisionCallback<CKingJellyfish>(ECollision_Result::Collision, this, &CKingJellyfish::AttackCollision);
    m_DetectCollision->SetCollisionCallback<CKingJellyfish>(ECollision_Result::Collision, this, &CKingJellyfish::DetectCollision);
    m_PoolCollision->SetCollisionCallback<CKingJellyfish>(ECollision_Result::Collision, this, &CKingJellyfish::PoolCollision);

    //std::string& Name = m_Mesh->GetSkeleton()->GetBone();

    //m_EffectMesh->GetSkeleton()->

    //m_EffectMesh->SetMaterial(0, "EffectTexture");
   // m_EffectMesh->SetMaterial(0, "DefaultTexture");

   // m_Mesh->SetWorldPosition(500.f, 500.f, 0.f);

    //m_Mesh->AddChild(m_EffectMesh);
    //m_Mesh->AddChild(m_AttackCollision);


    //m_EffectMesh->SetRelativeScale(1.25f, 1.25f);

    m_Animation = m_Mesh->SetAnimation<CAnimation>("KingJellyfishAnimation");

    m_Animation->AddAnimation("KingJellyfish_Idle", "KingJellyfish_Idle", 1.f, 0.5f, true);
    m_Animation->AddAnimation("KingJellyfish_Angry", "KingJellyfish_Angry", 1.f, 0.5f, true);
    m_Animation->AddAnimation("KingJellyfish_Attack", "KingJellyfish_Attack", 1.f, 0.5f, false);
    m_Animation->AddAnimation("KingJellyfish_Damage", "KingJellyfish_Damage", 1.f, 0.5f, true);
    m_Animation->AddAnimation("KingJellyfish_OnGround", "KingJellyfish_OnGround", 1.f, 0.5, true);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, false);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish1", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, false);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish2", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, false);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish3", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, false);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish4", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, false);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish5", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, false);



    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Attack", this, &CKingJellyfish::OnGround);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Damage", this, &CKingJellyfish::Angry);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_SpawnJellyfish", this, &CKingJellyfish::SpawnJellyfish);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_SpawnJellyfish1", this, &CKingJellyfish::Angry);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_SpawnJellyfish2", this, &CKingJellyfish::Angry);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_SpawnJellyfish3", this, &CKingJellyfish::Angry);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_SpawnJellyfish4", this, &CKingJellyfish::Angry);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_SpawnJellyfish5", this, &CKingJellyfish::Angry);


    //CSkeleton* Skeleton = m_Mesh->GetSkeleton();

    //m_EffectMesh->GetAnimation()->SetSkeleton(Skeleton);

   // m_BT = new CKingJellyfishBT;

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Idle");
    CResourceManager::GetInst()->SetVolume(2.f);

    m_PoolPos = m_Scene->FindObject("Pool")->GetWorldPos();

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

    CGameObject* Player = m_Scene->FindObject("Player");

    if (!Player)
    {
        return;
    }

    if (Player)
    {
        m_Idle = true;
    }

    if (m_Idle)
    {
        Idle();
    }

    if (m_Electric)
    {
        m_AttackTime += DeltaTime / DeltaTime;

        m_Mesh->AddWorldPosition(0.f, 0.f, 0.f);

        if (m_AttackTime > 3.f)
        {
            m_ElectricRing = m_Scene->CreateObject<CElectricRing>("ElectricRing");

            m_AttackTime = 0.f;

            m_OnGround = true;

            m_Electric = false;
        }
    }

    if (m_OnGround)
    {
        m_Mesh->AddWorldPosition(0.f, 0.f, 0.f);

        m_AttackTime += DeltaTime / DeltaTime;

        if (m_AttackTime > 3.f)
        {
            m_ReSpawn = true;

            m_OnGround = false;
        }
    }

    if (m_ReSpawn)
    {
        ReSpawn();
    }

    if (m_SpawnJellyfish)
    {
        m_Mesh->AddWorldPosition(0.f, 0.f, 0.f);
    }

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
    //    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Charge");

    //    if (!m_ElectricRing)
    //    {
    //        m_ElectricRing = m_Scene->CreateObject<CElectricRing>("ElectricRing");

    //        m_OnGround = true;
    //    }
    //}
    //
    //if (m_OnGround)
    //{
    //    AddWorldPosition(0.f, 0.f, 0.f);
    //}

    //if (m_SpawnJellyfish)
    //{
    // /*   for (int i = 0; i < 12; ++i)
    //    {
    //        m_AttackTime += DeltaTime / DeltaTime;

    //        if (m_AttackTime > 1.f)
    //        {
    //           m_Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
    //           m_Jellyfish->AddWorldRotationX(60.f * i);
    //           m_Jellyfish->AddWorldRotationZ(60.f * i);

    //           m_AttackTime = 0.f;
    //        }

    //    }*/

    //   //m_Jellyfish->AddWorldPosition(100.f * DeltaTime, 0.f, 100.f * DeltaTime);

    //    CJellyfish* Jellyfish1 = m_Scene->CreateObject<CJellyfish>("Jellyfish1");
    //    Jellyfish1->SetWorldRotationX(60.f);

    //    CJellyfish* Jellyfish2 = m_Scene->CreateObject<CJellyfish>("Jellyfish2");
    //    Jellyfish2->SetWorldRotationX(90.f);

    //    m_SpawnJellyfish = false;
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

void CKingJellyfish::AttackCollision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::PlayerAttack)
    {
        m_Electric = false;

        m_OnGround = false;

        Damage();

        m_Angry = true;

        m_CollisionCount++;

        if (m_CollisionCount == 3)
        { 
            m_Mesh->Destroy();
        }
    }
}


void CKingJellyfish::DetectCollision(const CollisionResult& result) 
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        m_Idle = false;

        Attack();
    }
}

void CKingJellyfish::PoolCollision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Pool)
    {
        m_Angry = false;

        m_ReSpawn = false;

        m_SpawnJellyfish = true;

        m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish");

        CResourceManager::GetInst()->SoundPlay("KingJellyfish_SpawnJellyfish");
    }
}

void CKingJellyfish::Idle()
{
    m_Animation->ChangeAnimation("KingJellyfish_Idle");

    Vector3 Dir = m_PlayerPos - m_MonsterPos;

    Dir.y = 0.f;

    Dir.Normalize();

    m_Mesh->AddWorldPosition(Dir * g_DeltaTime * 130.f);
}

void CKingJellyfish::Damage()
{
    m_Animation->ChangeAnimation("KingJellyfish_Damage");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Damage");
}

void CKingJellyfish::Angry()
{
    m_Animation->ChangeAnimation("KingJellyfish_Angry");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Angry");
}

void CKingJellyfish::Attack()
{
    m_Animation->ChangeAnimation("KingJellyfish_Attack");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_OnGround");

    m_Mesh->AddWorldPosition(0.f, 0.f, 0.f);
}

void CKingJellyfish::OnGround()
{
    m_Animation->ChangeAnimation("KingJellyfish_OnGround");

    m_Mesh->AddWorldPosition(0.f, 0.f, 0.f);

    m_Electric = true;
}

void CKingJellyfish::SpawnJellyfish()
{
    CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
    Jellyfish->AddWorldPosition(Vector3(1.f, 0.f, 0.f) * 100.f * g_DeltaTime);

    m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish1");
}

void CKingJellyfish::SpawnJellyfish1()
{
    CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
    Jellyfish->AddWorldPosition(Vector3(0.7f, 0.f, 0.3f) * 100.f * g_DeltaTime);

    m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish2");
}

void CKingJellyfish::SpawnJellyfish2()
{
    CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
    Jellyfish->AddWorldPosition(Vector3(0.5f, 0.f, 0.5f) * 100.f * g_DeltaTime);

    m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish3");
}

void CKingJellyfish::SpawnJellyfish3()
{
    CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
    Jellyfish->AddWorldPosition(Vector3(0.3f, 0.f, 0.7f) * 100.f * g_DeltaTime);

    m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish4");
}

void CKingJellyfish::SpawnJellyfish4()
{
    CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
    Jellyfish->AddWorldPosition(Vector3(0.f, 0.f, 1.f) * 100.f * g_DeltaTime);

    m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish5");
}

void CKingJellyfish::SpawnJellyfish5()
{
    m_Idle = true;
}

void CKingJellyfish::ReSpawn()
{
    m_Animation->ChangeAnimation("KingJellyfish_Idle");

    Vector3 Dir = m_PoolPos - m_MonsterPos;

    Dir.y = 0.f;

    Dir.Normalize();

    m_Mesh->AddWorldPosition(Dir * g_DeltaTime * 100.f);
}
