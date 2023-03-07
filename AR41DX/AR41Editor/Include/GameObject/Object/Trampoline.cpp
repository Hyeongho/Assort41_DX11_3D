#include "Trampoline.h"
#include "Component/ColliderCube.h"
#include "Component/StaticMeshComponent.h"

CTrampoline::CTrampoline()
{
}

CTrampoline::CTrampoline(const CTrampoline& Obj)
	: CGameObject(Obj)
{
}

CTrampoline::~CTrampoline()
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Cube = (CColliderCube*)FindComponent("Cube");
}

void CTrampoline::Start()
{
	CGameObject::Start();
}

bool CTrampoline::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Cube = CreateComponent<CColliderCube>("Cube");

	SetRootComponent(m_Mesh);

	//m_Mesh->SetMesh("Trampoline");

	return true;
}

void CTrampoline::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTrampoline::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTrampoline* CTrampoline::Clone() const
{
	return new CTrampoline(*this);
}

void CTrampoline::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTrampoline::Load(FILE* File)
{
	CGameObject::Load(File);
}
