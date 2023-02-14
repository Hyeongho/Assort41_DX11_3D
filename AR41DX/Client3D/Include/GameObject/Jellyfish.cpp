#include "Jellyfish.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CJellyfish::CJellyfish()
{
    SetTypeID<CJellyfish>();

    m_ObjectTypeName = "Jellyfish";
}

CJellyfish::CJellyfish(const CJellyfish& Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
}

CJellyfish::~CJellyfish()
{
}

void CJellyfish::Start()
{
    CGameObject::Start();
}

bool CJellyfish::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Jellyfish");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("JellyfishAnimation");

    m_Animation->AddAnimation("Jellyfish_Attack", "Jellyfish_Attack", 1.f, 1.f, true);
    m_Animation->AddAnimation("Jellyfish_Death", "Jellyfish_Death", 1.f, 1.f, true);

    return true;
}

void CJellyfish::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    // ���ĸ� ���ʹ� Idle ����� ���� ���� ������ Attack ����� ���
    m_Animation->ChangeAnimation("Jellyfish_Attack");
}

void CJellyfish::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CJellyfish* CJellyfish::Clone() const
{
    return new CJellyfish(*this);
}

void CJellyfish::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CJellyfish::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CJellyfish::Attack()
{ 
    // �÷��̾�� �浹���� �� ���
    // CResourceManager::GetInst()->SoundPlay("Jellyfish_Attack");
}

void CJellyfish::Death()
{
    m_Animation->ChangeAnimation("Jellyfish_Death");
}
