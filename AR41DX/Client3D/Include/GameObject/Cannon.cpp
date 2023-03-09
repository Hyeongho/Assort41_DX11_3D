#include "Cannon.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"

CCannon::CCannon()
{
}

CCannon::CCannon(const CCannon& Obj)
{
}

CCannon::~CCannon()
{
}

void CCannon::Start()
{
	CGameObject::Start();
}

bool CCannon::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	return true;
}

void CCannon::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CCannon::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CCannon* CCannon::Clone() const
{
	return new CCannon(*this);
}

void CCannon::Save(FILE* File)
{
}

void CCannon::Load(FILE* File)
{
}
