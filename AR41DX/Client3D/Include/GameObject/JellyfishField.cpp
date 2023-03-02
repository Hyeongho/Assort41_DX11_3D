#include "JellyfishField.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Scene/Scene.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "JellyFishField.h"


CJellyfishField::CJellyfishField()
{
    SetTypeID<CJellyfishField>();

    m_ObjectTypeName = "JellyfishField";
}

CJellyfishField::CJellyfishField(const CJellyfishField& Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CJellyfishField::~CJellyfishField()
{
}

void CJellyfishField::Start()
{
    CGameObject::Start();
}

bool CJellyfishField::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

    m_Mesh->SetMesh("JellyfishField");

    return true;
}

void CJellyfishField::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CJellyfishField::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CJellyfishField* CJellyfishField::Clone() const
{
    return new CJellyfishField(*this);
}

void CJellyfishField::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CJellyfishField::Load(FILE* File)
{
    CGameObject::Load(File);
}
