#include "CBL_BaseMesh.h"

#include "Component/StaticMeshComponent.h"
#include "Device.h"

CCBL_BaseMesh::CCBL_BaseMesh()
{
	SetTypeID<CCBL_BaseMesh>();

	m_ObjectTypeName = "CBL_BaseMesh";
}

CCBL_BaseMesh::CCBL_BaseMesh(const CCBL_BaseMesh& Obj)
	: CGameObject(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CCBL_BaseMesh::~CCBL_BaseMesh()
{
}

void CCBL_BaseMesh::Start()
{
	CGameObject::Start();
}

bool CCBL_BaseMesh::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("CBL_BaseMesh");


    return true;
}

void CCBL_BaseMesh::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CCBL_BaseMesh::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CCBL_BaseMesh* CCBL_BaseMesh::Clone() const
{
    return new CCBL_BaseMesh(*this);
}

void CCBL_BaseMesh::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CCBL_BaseMesh::Load(FILE* File)
{
	CGameObject::Load(File);
}
