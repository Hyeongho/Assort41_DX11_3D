#include "JellyfishFieldObj.h"
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

CJellyfishFieldObj::CJellyfishFieldObj()
{
	SetTypeID<CJellyfishFieldObj>();

	m_ObjectTypeName = "JellyfishFieldObj";
}

CJellyfishFieldObj::CJellyfishFieldObj(const CJellyfishFieldObj& Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CJellyfishFieldObj::~CJellyfishFieldObj()
{
}

void CJellyfishFieldObj::Start()
{
	CGameObject::Start();
}

bool CJellyfishFieldObj::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	m_Mesh->SetMesh("JellyfishFieldObj");

	return true;
}

void CJellyfishFieldObj::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CJellyfishFieldObj::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CJellyfishFieldObj* CJellyfishFieldObj::Clone() const
{
	return  new CJellyfishFieldObj(*this);;
}

void CJellyfishFieldObj::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CJellyfishFieldObj::Load(FILE* File)
{
	CGameObject::Load(File);
}
