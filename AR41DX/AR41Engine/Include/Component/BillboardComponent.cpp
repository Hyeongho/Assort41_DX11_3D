#include "BillboardComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"
#include "../Resource/ResourceManager.h"
#

CBillboardComponent::CBillboardComponent()
{
	SetTypeID<CBillboardComponent>();

	m_ComponentTypeName = "BillboardComponent";

	SetRenderLayerName("Particle");
}

CBillboardComponent::CBillboardComponent(const CBillboardComponent& component) :
	CPrimitiveComponent(component)
{
}

CBillboardComponent::~CBillboardComponent()
{
}

void CBillboardComponent::Start()
{
	CPrimitiveComponent::Start();
}

bool CBillboardComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}

	SetMesh("CenterUVRect");
	SetMaterial(0, "Billboard");
	SetRelativeScale(500.f, 500.f, 1.f);

	return true;
}

void CBillboardComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	Vector3	CameraPos = m_Scene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	Vector3	View = CameraPos - GetWorldPos();
	View.Normalize();

	Vector3	OriginDir(0.f, 0.f, -1.f);

	m_Transform->SetWorldRotationAxis(OriginDir, View);
}

void CBillboardComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CBillboardComponent::Render()
{
	CPrimitiveComponent::Render();
}

CBillboardComponent* CBillboardComponent::Clone() const
{
	return new CBillboardComponent(*this);
}

void CBillboardComponent::Save(FILE* File)
{
	CPrimitiveComponent::Save(File);
}

void CBillboardComponent::Load(FILE* File)
{
	CPrimitiveComponent::Load(File);
}
