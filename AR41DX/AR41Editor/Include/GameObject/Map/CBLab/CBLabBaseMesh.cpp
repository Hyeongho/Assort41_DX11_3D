#include "CBLabBaseMesh.h"

#include "Component/StaticMeshComponent.h"


CCBLabBaseMesh::CCBLabBaseMesh()
{
	SetTypeID<CCBLabBaseMesh>();

	m_ObjectTypeName = "CBLabBaseMesh";
}

CCBLabBaseMesh::CCBLabBaseMesh(const CCBLabBaseMesh& Obj)
	: CGameObject(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CCBLabBaseMesh::~CCBLabBaseMesh()
{
}

void CCBLabBaseMesh::Start()
{
	CGameObject::Start();
}

bool CCBLabBaseMesh::Init()
{
	CGameObject::Init();


	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	m_Mesh->SetMesh("CBLab_Ground1");


	return true;
}

void CCBLabBaseMesh::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CCBLabBaseMesh::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CCBLabBaseMesh* CCBLabBaseMesh::Clone() const
{
	return new CCBLabBaseMesh(*this);
}

void CCBLabBaseMesh::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CCBLabBaseMesh::Load(FILE* File)
{
	CGameObject::Load(File);
}
