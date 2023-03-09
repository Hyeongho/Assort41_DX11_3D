#include "Pufferfish.h"
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

CPufferfish::CPufferfish()
{
}

CPufferfish::CPufferfish(const CPufferfish& Obj)
{
}

CPufferfish::~CPufferfish()
{
}

void CPufferfish::Start()
{
	CGameObject::Start();
}

bool CPufferfish::Init()
{
	CGameObject::Init();

	return true;
}

void CPufferfish::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPufferfish::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPufferfish* CPufferfish::Clone() const
{
	return new CPufferfish(*this);
}

void CPufferfish::Save(FILE* File)
{
}

void CPufferfish::Load(FILE* File)
{
}
