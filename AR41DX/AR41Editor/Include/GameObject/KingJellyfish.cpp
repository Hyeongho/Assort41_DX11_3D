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
#include "../UI/KingJellyfishOutroUI.h"


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
    , m_JellyfishCount(0)
    , m_MonsterPos(0.f, 0.f, 0.f)
    , m_PlayerPos(0.f, 0.f, 0.f)
    , m_PoolPos(0.f, 0.f, 0.f)
{
    SetTypeID<CKingJellyfish>();

    m_ObjectTypeName = "KingJellyfish";
}

CKingJellyfish::CKingJellyfish(const CKingJellyfish& Obj) : CGameObject(Obj)
, m_Animation(Obj.m_Animation)
, m_AttackTime(0.f)
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
, m_JellyfishCount(0)
, m_MonsterPos(0.f, 0.f, 0.f)
, m_PlayerPos(0.f, 0.f, 0.f)
, m_PoolPos(0.f, 0.f, 0.f)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_AttackCollision = (CColliderOBB3D*)FindComponent("Cube");
    m_DetectCollision = (CColliderOBB3D*)FindComponent("Cube");
    m_PoolCollision = (CColliderOBB3D*)FindComponent("Cube");
}

CKingJellyfish::~CKingJellyfish()
{
}

void CKingJellyfish::Start()
{
    CGameObject::Start();

    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Attack", this, &CKingJellyfish::OnGround);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Damage", this, &CKingJellyfish::Angry);
    m_Animation->SetCurrentEndFunction<CKingJellyfish>("KingJellyfish_Angry", this, &CKingJellyfish::Cutscene);

    m_AttackCollision->SetCollisionCallback<CKingJellyfish>(ECollision_Result::Collision, this, &CKingJellyfish::AttackCollision);
    m_DetectCollision->SetCollisionCallback<CKingJellyfish>(ECollision_Result::Collision, this, &CKingJellyfish::DetectCollision);
    m_PoolCollision->SetCollisionCallback<CKingJellyfish>(ECollision_Result::Collision, this, &CKingJellyfish::PoolCollision);

    if (m_Scene)
    {
        m_PoolPos = m_Scene->FindObject("Pool")->GetWorldPos();
    }
}

bool CKingJellyfish::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
    m_AttackCollision = CreateComponent<CColliderOBB3D>("Cube");
    m_DetectCollision = CreateComponent<CColliderOBB3D>("Cube");
    m_PoolCollision = CreateComponent<CColliderOBB3D>("Cube");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("KingJellyfish");

    m_Mesh->SetWorldPositionY(250.f);

    m_Mesh->AddChild(m_AttackCollision);
    m_Mesh->AddChild(m_DetectCollision);
    m_Mesh->AddChild(m_PoolCollision);

    m_AttackCollision->SetBoxHalfSize(350.f, 200.f, 350.f);
    m_AttackCollision->SetRelativePosition(0.f, 400.f);

    m_DetectCollision->SetBoxHalfSize(700.f, 500.f, 700.f);
    m_DetectCollision->SetRelativePosition(0.f, 500.f);

    m_PoolCollision->SetBoxHalfSize(50.f, 200.f, 50.f);
    m_PoolCollision->SetRelativePosition(0.f, 200.f);

    m_AttackCollision->SetCollisionProfile("Monster");
    m_DetectCollision->SetCollisionProfile("KingJellyfish");
    m_PoolCollision->SetCollisionProfile("Monster");


    //std::string& Name = m_Mesh->GetSkeleton()->GetBone();

    //m_EffectMesh->GetSkeleton()->

    //m_EffectMesh->SetMaterial(0, "EffectTexture");
   // m_EffectMesh->SetMaterial(0, "DefaultTexture");

   // m_Mesh->SetWorldPosition(500.f, 500.f, 0.f);

    //m_Mesh->AddChild(m_EffectMesh);
    //m_Mesh->AddChild(m_AttackCollision);

    m_Animation = m_Mesh->SetAnimation<CAnimation>("KingJellyfishAnimation");

    m_Animation->AddAnimation("KingJellyfish_Idle", "KingJellyfish_Idle", 1.f, 0.5f, true);
    m_Animation->AddAnimation("KingJellyfish_Angry", "KingJellyfish_Angry", 1.f, 0.5f, false);
    m_Animation->AddAnimation("KingJellyfish_Attack", "KingJellyfish_Attack", 1.f, 0.5f, false);
    m_Animation->AddAnimation("KingJellyfish_Damage", "KingJellyfish_Damage", 1.f, 0.5f, false);
    m_Animation->AddAnimation("KingJellyfish_OnGround", "KingJellyfish_OnGround", 1.f, 0.5, true);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, true);

    return true;
}

void CKingJellyfish::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    if (m_Scene)
    {
        m_MonsterPos = m_Mesh->GetWorldPos();

        m_PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();
    }

    //float MeshMaxY = m_Mesh->GetMesh()->GetMax().y;

    //m_EffectMesh->SetMax(Vector3(m_EffectMesh.Get()->GetMax().x, MeshMaxY, m_EffectMesh.Get()->GetMax().z));

    //float MeshMinY = m_Mesh->GetMesh()->GetMin().y;

    //m_EffectMesh->SetMin(Vector3(m_EffectMesh.Get()->GetMin().x, MeshMaxY, m_EffectMesh.Get()->GetMin().z));

    if (m_CollisionCount == 2)
    {
        Destroy();
    }

}

void CKingJellyfish::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    if (m_Idle)
    {
        Idle();
    }

    if (m_Electric)
    {
        m_Mesh->AddWorldPosition(0.f, 0.f, 0.f);

        m_ElectricRing = m_Scene->CreateObject<CElectricRing>("ElectricRing");

        m_AttackTime = 0.f;

        m_OnGround = true;

        m_Electric = false;
    }

    if (m_OnGround)
    {
        m_Mesh->AddWorldPosition(0.f, 0.f, 0.f);

        m_AttackTime += DeltaTime;

        if (m_AttackTime > 6.f)
        {
            m_ReSpawn = true;

            m_AttackTime = 0.f;

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

        m_AttackTime += g_DeltaTime;

        if (m_AttackTime > 1.f && m_JellyfishCount < 1)
        {
            CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
            Jellyfish->SetCount(1);
            Jellyfish->SetBoss(true);
            ++m_JellyfishCount;
        }

        if (m_AttackTime > 2.5f && m_JellyfishCount >= 1 && m_JellyfishCount < 2)
        {
            CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
            Jellyfish->SetCount(2);
            ++m_JellyfishCount;
        }

        if (m_AttackTime > 4.f && m_JellyfishCount >= 2 && m_JellyfishCount < 3)
        {
            CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
            Jellyfish->SetCount(3);
            ++m_JellyfishCount;
        }

        if (m_AttackTime > 5.5f && m_JellyfishCount >= 3 && m_JellyfishCount < 4)
        {
            CJellyfish* Jellyfish = m_Scene->CreateObject<CJellyfish>("Jellyfish");
            Jellyfish->SetCount(4);
            ++m_JellyfishCount;
        }

        if (m_AttackTime > 7.f)
        {
            m_Idle = true;

            m_AttackTime = 0.f;

            m_SpawnJellyfish = false;

            m_JellyfishCount = 0;
        }

    }

    if (m_Angry)
    {
        Vector3 Dir = m_PoolPos - m_MonsterPos;

        Dir.y = 0.f;

        Dir.Normalize();

        float Degree = atan2(GetWorldPos().z - m_PoolPos.z, GetWorldPos().x - m_PoolPos.x);
        Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

        SetWorldRotationY(Degree);

        AddWorldPosition(Dir * DeltaTime * 100.f);
    }
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

    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_AttackCollision = (CColliderOBB3D*)FindComponent("Cube");
    m_DetectCollision = (CColliderOBB3D*)FindComponent("Cube");
    m_PoolCollision = (CColliderOBB3D*)FindComponent("Cube");
}

void CKingJellyfish::AttackCollision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Name == "PlayerAttack")
    {
        if (m_Electric)
        {
            m_Electric = false;

            Damage();

          //  m_Angry = true;

           // m_CollisionCount++;
        }

        if (m_OnGround)
        {
            m_OnGround = false;

            Damage();

           // m_Angry = true;

           // m_CollisionCount++;
        }


    }
}


void CKingJellyfish::DetectCollision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Name == "Player")
    {
        if (m_Idle)
        {
            m_Idle = false;

            Attack();
        }
    }
}

void CKingJellyfish::PoolCollision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Name == "Pool")
    {
        if (m_Angry)
        {
            m_Angry = false;

            m_SpawnJellyfish = true;

            m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish");

            CResourceManager::GetInst()->SoundPlay("KingJellyfish_SpawnJellyfish");
        }

        if (m_ReSpawn)
        {
            m_ReSpawn = false;

            m_SpawnJellyfish = true;

            m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish");

            CResourceManager::GetInst()->SoundPlay("KingJellyfish_SpawnJellyfish");
        }
    }
}

void CKingJellyfish::Idle()
{
    m_Animation->ChangeAnimation("KingJellyfish_Idle");

    Vector3 Dir = m_PlayerPos - m_MonsterPos;

    Dir.y = 0.f;

    Dir.Normalize();

    float Degree = atan2(GetWorldPos().z - m_PlayerPos.z, GetWorldPos().x - m_PlayerPos.x);
    Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

    SetWorldRotationY(Degree);

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

    //m_Angry = true;
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

void CKingJellyfish::ReSpawn()
{
    m_Animation->ChangeAnimation("KingJellyfish_Idle");

    Vector3 Dir = m_PoolPos - m_MonsterPos;

    Dir.y = 0.f;

    Dir.Normalize();

    float Degree = atan2(GetWorldPos().z - m_PoolPos.z, GetWorldPos().x - m_PoolPos.x);
    Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

    SetWorldRotationY(Degree);

    AddWorldPosition(Dir * g_DeltaTime * 100.f);
}

void CKingJellyfish::Cutscene()
{
   // m_Scene->GetViewport()->CreateUIWindow<CKingJellyfishOutroUI>("KingJellyfishOutroUI");

   // m_Scene->GetViewport()->FindUIWindow<CKingJellyfishOutroUI>("KingJellyfishOutroUI")->Stop();
}
