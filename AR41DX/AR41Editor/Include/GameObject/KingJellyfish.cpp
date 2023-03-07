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

CKingJellyfish::CKingJellyfish()
    : m_AttackTime(0.f)
{
    SetTypeID<CKingJellyfish>();

    m_ObjectTypeName = "KingJellyfish";
}

CKingJellyfish::CKingJellyfish(const CKingJellyfish& Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
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

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("KingJellyfish");
    m_Mesh->SetWorldPosition(0.f, 0.f, 0.f);

    m_Animation = m_Mesh->SetAnimation<CAnimation>("KingJellyfishAnimation");

    m_Animation->AddAnimation("KingJellyfish_Idle", "KingJellyfish_Idle", 1.f, 0.5f, true);
    m_Animation->AddAnimation("KingJellyfish_Angry", "KingJellyfish_Angry", 1.f, 0.5f, false);
    m_Animation->AddAnimation("KingJellyfish_Attack", "KingJellyfish_Attack", 1.f, 0.5f, false);
    m_Animation->AddAnimation("KingJellyfish_Damage", "KingJellyfish_Damage", 1.f, 0.5f, false);
    m_Animation->AddAnimation("KingJellyfish_OnGround", "KingJellyfish_OnGround", 1.f, 0.5, false);
    m_Animation->AddAnimation("KingJellyfish_SpawnJellyfish", "KingJellyfish_SpawnJellyfish", 1.f, 0.5, false);

    return true;
}

void CKingJellyfish::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Idle");
    CResourceManager::GetInst()->SetVolume(2.f);

    m_Animation->ChangeAnimation("KingJellyfish_Idle");
}

void CKingJellyfish::PostUpdate(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
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

void CKingJellyfish::Damage()
{
    m_Animation->ChangeAnimation("KingJellyfish_Damage");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Damage");
}

void CKingJellyfish::Angry()
{
    m_Animation->ChangeAnimation("KingJellyfish_Angry");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_Angry");

    Attack();
}

void CKingJellyfish::Attack()
{
    m_Animation->ChangeAnimation("KingJellyfish_Attack");

    m_AttackTime = 0.f;

    m_AttackTime += g_DeltaTime;

    if (m_AttackTime >= 1.5f)
    {
        CResourceManager::GetInst()->SoundPlay("KingJellyfish_Charge");

        m_AttackTime = 0.f;
    }
}

void CKingJellyfish::OnGround()
{
    m_Animation->ChangeAnimation("KingJellyfish_OnGround");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_OnGround");
}

void CKingJellyfish::SpawnJellyfish()
{
    m_Animation->ChangeAnimation("KingJellyfish_SpawnJellyfish");

    CResourceManager::GetInst()->SoundPlay("KingJellyfish_SpawnJellyfish");
}
