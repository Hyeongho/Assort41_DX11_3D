#include "JellyfishElectric.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderSphere3D.h"
#include "Jellyfish.h"
#include "Scene/Scene.h"

CJellyfishElectric::CJellyfishElectric() :
    m_Attack(false),
    m_AttackTime(0.f)
{
    SetTypeID<CJellyfishElectric>();

    m_ObjectTypeName = "JellyfishElectric";
}

CJellyfishElectric::CJellyfishElectric(const CJellyfishElectric& Obj)
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

    SetRootComponent(m_Mesh);

    m_Mesh->SetWorldScale(50.f, 50.f, 50.f);
    //m_Mesh->GetMaterial(0)->SetOpacity(0.1f);
    //m_Mesh->GetMaterial(0)->SetRenderState("AlphaBlecnd");

    m_Mesh->AddChild(m_Collider);

    m_Collider->SetRelativeScale(0.8f, 0.8f, 0.8f);
    m_Collider->SetCollisionProfile("Monster");

    m_Collider->SetRelativePosition(0.f, 0.f, 0.f);

    return true;
}

void CJellyfishElectric::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    if (m_Attack)
    {
        m_AttackTime += DeltaTime;

        if (m_AttackTime > 2.f)
        {
            Destroy();
        }
    }
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
        m_Attack = true;
    }
}
