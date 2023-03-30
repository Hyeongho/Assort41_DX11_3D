#include "Pool.h"
#include "Component/ColliderCube.h"

CPool::CPool()
{
    SetTypeID<CPool>();

    m_ObjectTypeName = "Pool";
}

CPool::CPool(const CPool& Obj)
    : CGameObject(Obj)
{
    m_Cube = (CColliderCube*)FindComponent("Cube");
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

    m_Cube = CreateComponent<CColliderCube>("Cube");

    m_Cube->SetWorldPosition(0.f, 0.f, 0.f);

    m_Cube->SetWorldScale(200.f, 200.f, 200.f);

    m_Cube->SetCollisionProfile("Pool");

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
