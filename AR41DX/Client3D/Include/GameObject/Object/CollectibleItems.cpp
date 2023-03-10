#include "CollectibleItems.h"

#include "Scene/Scene.h"
#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CCollectibleItems::CCollectibleItems()
{
}

CCollectibleItems::CCollectibleItems(const CCollectibleItems& Obj) :
    CGameObject(Obj)
{
}

CCollectibleItems::~CCollectibleItems()
{
}

void CCollectibleItems::Start()
{
    CGameObject::Start();
}

bool CCollectibleItems::Init()
{
    CGameObject::Init();


    return true;
}

void CCollectibleItems::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CCollectibleItems::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CCollectibleItems* CCollectibleItems::Clone() const
{
    return new CCollectibleItems(*this);
}

void CCollectibleItems::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CCollectibleItems::Load(FILE* File)
{
    CGameObject::Load(File);
}
