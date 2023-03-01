#include "TaxiDriver.h"

#include "Component/StaticMeshComponent.h"

CTaxiDriver::CTaxiDriver()
{
}

CTaxiDriver::CTaxiDriver(const CTaxiDriver& Obj)
	: CGameObject(Obj)
{
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

	m_Mesh->SetMesh("Taxi");

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
