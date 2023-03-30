#include "JellyfishElectric.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderSphere3D.h"
#include "Jellyfish.h"
#include "Scene/Scene.h"

CJellyfishElectric::CJellyfishElectric() :
    m_Time(0.f)
{
    SetTypeID<CJellyfishElectric>();

    m_ObjectTypeName = "JellyfishElectric";
}

CJellyfishElectric::CJellyfishElectric(const CJellyfishElectric& Obj) : CGameObject(Obj),
    m_Time(0.f)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Collider = (CColliderSphere3D*)FindComponent("Collider");
}

CJellyfishElectric::~CJellyfishElectric()
{
}

void CJellyfishElectric::Start()
{
    CGameObject::Start();

    m_Collider->SetCollisionCallback<CJellyfishElectric>(ECollision_Result::Collision, this, &CJellyfishElectric::Collision);
}

bool CJellyfishElectric::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
    m_Collider = CreateComponent<CColliderSphere3D>("Collider");

    m_Mesh->SetMesh("Jellyfish_Electric");
    m_Mesh->SetMaterial(0, "Lightning");

    SetRootComponent(m_Mesh);

    m_Mesh->SetWorldScale(500.f, 500.f, 500.f);

    m_Mesh->AddChild(m_Collider);

    m_Collider->SetRelativeScale(0.8f, 0.8f, 0.8f);
    m_Collider->SetCollisionProfile("MonsterAttack");

    m_Collider->SetRelativePosition(0.f, 0.f, 0.f);

    return true;
}

void CJellyfishElectric::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    m_Time += DeltaTime;

    if (m_Time > 2.f)
        SetLifeTime(DeltaTime);
}

void CJellyfishElectric::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CJellyfishElectric* CJellyfishElectric::Clone() const
{
    return new CJellyfishElectric(*this);
}

void CJellyfishElectric::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CJellyfishElectric::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CJellyfishElectric::Collision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Name == "Player")
    {
        result.Dest->GetOwner()->InflictDamage();
    }
}

