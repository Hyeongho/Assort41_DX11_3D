#include "Jellyfish.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Component/ColliderCube.h"
#include "KingJellyfish.h"
#include "JellyfishElectric.h"

CJellyfish::CJellyfish() :
    m_MonsterPos(0.f, 0.f, 0.f)
{
    SetTypeID<CJellyfish>();

    m_ObjectTypeName = "Jellyfish";
}

CJellyfish::CJellyfish(const CJellyfish& Obj)
    : CGameObject(Obj)
    , m_Animation(Obj.m_Animation)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Cube = (CColliderCube*)FindComponent("Cube");
}

CJellyfish::~CJellyfish()
{
}

void CJellyfish::Start()
{
    CGameObject::Start();

    m_Cube->SetCollisionCallback<CJellyfish>(ECollision_Result::Collision, this, &CJellyfish::Collision);

    m_Animation->SetCurrentEndFunction<CJellyfish>("Jellyfish_Death", this, &CJellyfish::Dead);
}

bool CJellyfish::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
    m_Cube = CreateComponent<CColliderCube>("Cube");

    SetRootComponent(m_Mesh);

    m_Mesh->AddChild(m_Cube);

    m_Mesh->SetMesh("Jellyfish");

    m_Cube->SetCubeSize(100.f, 100.f, 100.f);

    m_Cube->SetCollisionProfile("Monster");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("JellyfishAnimation");

    m_Animation->AddAnimation("Jellyfish_Attack", "Jellyfish_Attack", 1.f, 1.f, true);
    m_Animation->AddAnimation("Jellyfish_Death", "Jellyfish_Death", 1.f, 1.f, false);

    CGameObject* KingJellyfish = m_Scene->FindObject("KingJellyfish");

    if (KingJellyfish)
    {
        m_Mesh->SetWorldPosition(KingJellyfish->GetWorldPos());
    }

    return true;
}

void CJellyfish::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    // 해파리 몬스터는 Idle 모션이 따로 없기 때문에 Attack 모션을 사용
    m_Animation->ChangeAnimation("Jellyfish_Attack");

    CGameObject* Player = m_Scene->FindObject("Player");

    Vector3 PlayerPos = Player->GetWorldPos();
    m_MonsterPos = m_Mesh->GetWorldPos();

    if (!Player)
    {
        return;
    }

    Vector3 Dir = PlayerPos - m_MonsterPos;

    Dir.y = 0.f;

    Dir.Normalize();

    float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
    Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

    SetWorldRotationY(Degree);
    SetWorldRotationX(-35.f);

    AddWorldPosition(Dir * g_DeltaTime * 100.f);
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

void CJellyfish::Collision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Name == "Player")
    {
        CJellyfishElectric* JellyfishElectric = m_Scene->CreateObject<CJellyfishElectric>("JellyfishElectric");

        JellyfishElectric->SetWorldPosition(m_MonsterPos);

        CResourceManager::GetInst()->SoundPlay("Jellyfish_Attack");
    }

    if (result.Dest->GetCollisionProfile()->Name == "PlayerAttack")
    {
        m_Animation->ChangeAnimation("Jellyfish_Death");

    }
}

void CJellyfish::Dead()
{
    Destroy();
}

