#include "BikiniBottomBuildings.h"
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

CBikiniBottomBuildings::CBikiniBottomBuildings()
{
	SetTypeID<CBikiniBottomBuildings>();

	m_ObjectTypeName = "BikiniBottomHouse";
}

CBikiniBottomBuildings::CBikiniBottomBuildings(const CBikiniBottomBuildings& Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CBikiniBottomBuildings::~CBikiniBottomBuildings()
{
}

void CBikiniBottomBuildings::Start()
{
	CGameObject::Start();
}

bool CBikiniBottomBuildings::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	m_Mesh->SetMesh("RedTree");

	return true;
}

void CBikiniBottomBuildings::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBikiniBottomBuildings::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBikiniBottomBuildings* CBikiniBottomBuildings::Clone() const
{
	return  new CBikiniBottomBuildings(*this);;
}

void CBikiniBottomBuildings::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CBikiniBottomBuildings::Load(FILE* File)
{
	CGameObject::Load(File);
}
