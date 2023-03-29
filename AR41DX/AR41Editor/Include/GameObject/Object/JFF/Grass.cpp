#include "Grass.h"

#include "Component/StaticMeshComponent.h"

CGrass::CGrass()
{
    SetTypeID<CGrass>();

    m_ObjectTypeName = "Grass";

    m_RotLeft = false;
    m_RotRight = true;
}

CGrass::CGrass(const CGrass& Obj) :
    CGameObject(Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");

    m_RotLeft = Obj.m_RotLeft;
    m_RotRight = Obj.m_RotRight;
}

CGrass::~CGrass()
{
}

void CGrass::Start()
{
    CGameObject::Start();
}

bool CGrass::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);
    m_Mesh->SetMesh("Grass1");

    //int Rot = rand() % 360;
    //SetWorldRotationY((float)Rot);

    SetWorldScale(0.2f, 0.2f, 0.2f);

    return true;
}

void CGrass::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    return;

    int ZRot = (int)GetWorldRot().z;
    ZRot %= 360;

    if (ZRot == -3 && m_RotRight) {
        m_RotLeft = true;
        m_RotRight = false;
    }
    
    if (ZRot == -357 && m_RotLeft) {
        m_RotLeft = false;
        m_RotRight = true;
    }

    if (ZRot < -3 && ZRot > -356) 
        SetWorldRotationZ(-359.f);

    if (m_RotLeft)
        AddWorldRotationZ(g_DeltaTime * 10.f);
    if (m_RotRight)
        AddWorldRotationZ(g_DeltaTime * -10.f);
}

void CGrass::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CGrass* CGrass::Clone() const
{
    return new CGrass(*this);
}

void CGrass::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CGrass::Load(FILE* File)
{
    CGameObject::Load(File);
}
