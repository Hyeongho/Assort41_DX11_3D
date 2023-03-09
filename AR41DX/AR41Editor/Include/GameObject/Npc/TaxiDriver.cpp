#include "TaxiDriver.h"

#include "Component/StaticMeshComponent.h"

CTaxiDriver::CTaxiDriver()
{
	SetTypeID<CTaxiDriver>();

	m_ObjectTypeName = "TaxiDriver";

	m_DialogCount = 0;
	m_NpcType = ENpcList::Squidward;
	m_NpcMapPos = EMapList::Bikini_Bottom;
	m_EnableDialog = false;
	m_NpcMeshType = MeshType::Static;
}

CTaxiDriver::CTaxiDriver(const CTaxiDriver& Obj)
	: CNpc(Obj)
{
	m_StaticMesh = (CStaticMeshComponent*)FindComponent("Mesh");

	m_DialogCount = Obj.m_DialogCount;
	m_NpcType = Obj.m_NpcType;
	m_NpcMapPos = Obj.m_NpcMapPos;
	m_EnableDialog = Obj.m_EnableDialog;
	m_NpcMeshType = Obj.m_NpcMeshType;
}

CTaxiDriver::~CTaxiDriver()
{
}

void CTaxiDriver::Start()
{
	CGameObject::Start();
}

bool CTaxiDriver::Init()
{
	CGameObject::Init();

	m_StaticMesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_StaticMesh->SetMesh("Taxi");

	SetRootComponent(m_StaticMesh);
	
	return true;
}

void CTaxiDriver::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTaxiDriver::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTaxiDriver* CTaxiDriver::Clone() const
{
	return new CTaxiDriver(*this);
}

void CTaxiDriver::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTaxiDriver::Load(FILE* File)
{
	CGameObject::Load(File);
}