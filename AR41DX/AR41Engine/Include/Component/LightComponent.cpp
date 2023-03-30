
#include "LightComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/LightManager.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"

CLightComponent::CLightComponent() : m_CBuffer(nullptr)
{
	SetTypeID<CLightComponent>();

	m_ComponentTypeName = "LightComponent";

	m_LightInfo.Color = Vector4::White;
}

CLightComponent::CLightComponent(const CLightComponent& component) : CSceneComponent(component)
{
	m_LightInfo = component.m_LightInfo;
	if (component.m_CBuffer)
	{
		m_CBuffer = component.m_CBuffer->Clone();
	}
}

CLightComponent::~CLightComponent()
{
	SAFE_DELETE(m_CBuffer);
}

void CLightComponent::Destroy()
{
	CSceneComponent::Destroy();
}

void CLightComponent::Start()
{
	CSceneComponent::Start();

	m_CBuffer->SetLightInfo(m_LightInfo);

	m_Scene->GetLightManager()->AddLight(this);
}

bool CLightComponent::Init()
{
	CSceneComponent::Init();

	m_CBuffer = new CLightConstantBuffer;

	m_CBuffer->Init();

	return true;
}

void CLightComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CLightComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	if (m_LightInfo.LightType != (int)ELightType::Direction)
	{
		Vector3	Pos = GetWorldPos();

		// 뷰 공간으로 변환한다.
		Pos = Pos.TransformCoord(Camera->GetViewMatrix());

		m_LightInfo.Pos = Pos;

		m_CBuffer->SetLightPos(Pos);
	}

	if (m_LightInfo.LightType != (int)ELightType::Point)
	{
		Vector3	Dir = GetWorldAxis(AXIS::AXIS_Z);

		// 뷰 공간으로 변환한다.
		Dir = Dir.TransformNormal(Camera->GetViewMatrix());
		Dir.Normalize();

		m_LightInfo.Dir = Dir;

		m_CBuffer->SetLightDir(Dir);
	}
}

void CLightComponent::Render()
{
	CSceneComponent::Render();
}

CLightComponent* CLightComponent::Clone() const
{
	return new CLightComponent(*this);
}

void CLightComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	fwrite(&m_LightInfo, sizeof(LightCBuffer), 1, File);
}

void CLightComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	fread(&m_LightInfo, sizeof(LightCBuffer), 1, File);
}

void CLightComponent::SetShader()
{
	m_CBuffer->UpdateBuffer();
}
