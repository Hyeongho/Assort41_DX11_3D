#include "CBL_Floor.h"

#include "Component/StaticMeshComponent.h"
#include "Device.h"

CCBL_Floor::CCBL_Floor()
{
	SetTypeID<CCBL_Floor>();

	m_ObjectTypeName = "CBL_Platform";
}

CCBL_Floor::CCBL_Floor(const CCBL_Floor& Obj) :
	CGameObject(Obj)
{
	m_InnerMesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_OuterMesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CCBL_Floor::~CCBL_Floor()
{
}

void CCBL_Floor::Start()
{
	CGameObject::Start();
}

bool CCBL_Floor::Init()
{
	CGameObject::Init();

	m_InnerMesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_OuterMesh = CreateComponent<CStaticMeshComponent>("Mesh");

	SetRootComponent(m_InnerMesh);
	m_InnerMesh->AddChild(m_OuterMesh);

	m_InnerMesh->SetMesh("CBL_InnerFloor");
	m_OuterMesh->SetMesh("CBL_OuterFloor");


	//Vector2 Ratio = CDevice::GetInst()->GetHdRsRatio();
	//Ratio.x = 1.f / Ratio.x;
	//Ratio.y = 1.f / Ratio.y;

	//m_InnerMesh->SetRelativeScale(Ratio);
	//m_OuterMesh->SetRelativeScale(Ratio);


	return true;
}

void CCBL_Floor::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CCBL_Floor::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CCBL_Floor* CCBL_Floor::Clone() const
{
	return new CCBL_Floor(*this);
}

void CCBL_Floor::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CCBL_Floor::Load(FILE* File)
{
	CGameObject::Load(File);
}
