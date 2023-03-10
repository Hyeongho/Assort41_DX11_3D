#include "Sock.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CSock::CSock()
{
}

CSock::CSock(const CSock& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
}

CSock::~CSock()
{
}

void CSock::Start()
{
	CCollectibleItems::Start();
}

bool CSock::Init()
{
	CCollectibleItems::Init();

	return true;
}

void CSock::Update(float DeltaTime)
{
	CCollectibleItems::Update(DeltaTime);
}

void CSock::PostUpdate(float DeltaTime)
{
	CCollectibleItems::PostUpdate(DeltaTime);
}

CSock* CSock::Clone() const
{
	return new CSock(*this);
}

void CSock::Save(FILE* File)
{
	CCollectibleItems::Save(File);
}

void CSock::Load(FILE* File)
{
	CCollectibleItems::Load(File);
}

