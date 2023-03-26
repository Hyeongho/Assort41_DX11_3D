#include "Pool.h"
#include "Component/ColliderOBB3D.h"

CPool::CPool()
{
    SetTypeID<CPool>();

    m_ObjectTypeName = "Pool";
}

CPool::CPool(const CPool& Obj)
{
    m_Collider = (CColliderOBB3D*)FindComponent("Collider");
}

CPool::~CPool()
{
}

void CPool::Start()
{
    CGameObject::Start();
}

bool CPool::Init()
{
    CGameObject::Init();

    m_Collider = CreateComponent<CColliderOBB3D>("Collider");

    m_Collider->SetWorldPosition(0.f, 100.f, 0.f);

    m_Collider->SetBoxHalfSize(100.f, 100.f, 100.f);

    m_Collider->SetCollisionProfile("Pool");

    SetRootComponent(m_Collider);

    return true;
}

void CPool::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CPool::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CPool* CPool::Clone() const
{
    return new CPool(*this);
}

void CPool::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CPool::Load(FILE* File)
{
    CGameObject::Load(File);
}
