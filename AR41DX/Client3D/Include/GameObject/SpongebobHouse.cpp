#include "SpongebobHouse.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Input.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CSpongebobHouse::CSpongebobHouse()
{
	SetTypeID<CSpongebobHouse>();

	m_ObjectTypeName = "SpongebobHouse";
}

CSpongebobHouse::CSpongebobHouse(const CSpongebobHouse& Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CSpongebobHouse::~CSpongebobHouse()
{
}

void CSpongebobHouse::Start()
{
	CGameObject::Start();
}

bool CSpongebobHouse::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	m_Mesh->SetMesh("RedTree");

	return true;
}

void CSpongebobHouse::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CSpongebobHouse::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSpongebobHouse* CSpongebobHouse::Clone() const
{
	return  new CSpongebobHouse(*this);;
}

void CSpongebobHouse::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CSpongebobHouse::Load(FILE* File)
{
	CGameObject::Load(File);
}
