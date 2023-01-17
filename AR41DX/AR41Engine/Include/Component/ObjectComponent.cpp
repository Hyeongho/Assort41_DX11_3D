#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{
	m_ComponentType = ComponentType::Object;

	SetTypeID<CObjectComponent>();

	m_ComponentTypeName = "ObjectComponent";
}

CObjectComponent::CObjectComponent(const CObjectComponent& Obj)	:
	CComponent(Obj)
{
}

CObjectComponent::~CObjectComponent()
{
}

void CObjectComponent::Destroy()
{
	CComponent::Destroy();
}

void CObjectComponent::Start()
{
	CComponent::Start();
}

bool CObjectComponent::Init()
{
	if (!CComponent::Init())
		return false;

	return true;
}

void CObjectComponent::Update(float DeltaTime)
{
	CComponent::Update(DeltaTime);
}

void CObjectComponent::PostUpdate(float DeltaTime)
{
	CComponent::PostUpdate(DeltaTime);
}

void CObjectComponent::Render()
{
	CComponent::Render();
}

CObjectComponent* CObjectComponent::Clone() const
{
	return nullptr;
}

void CObjectComponent::Save(FILE* File)
{
	CComponent::Save(File);
}

void CObjectComponent::Load(FILE* File)
{
	CComponent::Load(File);
}
