#include "DecalComponent.h"
#include "../Resource/Material/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../GameObject/GameObject.h"

CDecalComponent::CDecalComponent()
{
	SetTypeID<CDecalComponent>();

	m_ComponentTypeName = "DecalComponent";

	SetRenderLayerName("Decal");

	m_FadeState = EDecalFadeState::None;
	m_FadeTime = 0.f;
	m_FadeInTime = 0.f;
	m_FadeOutTime = 0.f;
	m_FadeOutDelayTime = 0.f;
	m_FinishDelete = true;

	m_Fade = EFadeState::FadeIn;
}

CDecalComponent::CDecalComponent(const CDecalComponent& component) : CPrimitiveComponent(component)
{
#ifdef _DEBUG
	m_DebugMesh = component.m_DebugMesh;
	m_DebugMaterial = component.m_DebugMaterial;
#endif // _DEBUG
	m_FadeState = component.m_FadeState;
	m_FadeInTime = component.m_FadeInTime;
	m_FadeOutTime = component.m_FadeOutTime;
	m_FadeOutDelayTime = component.m_FadeOutDelayTime;
	m_FinishDelete = component.m_FinishDelete;
	m_FadeTime = 0.f;

	m_Fade = EFadeState::FadeIn;
}

CDecalComponent::~CDecalComponent()
{
}

void CDecalComponent::Start()
{
	CPrimitiveComponent::Start();

	switch (m_FadeState)
	{
	case EDecalFadeState::None:
		m_FadeTime = 0.f;
		break;
	case EDecalFadeState::FadeIn:
		m_FadeTime = 0.f;
		break;
	case EDecalFadeState::FadeOut:
		m_FadeTime = m_FadeOutTime;
		break;
	case EDecalFadeState::FadeInOut:
		m_FadeTime = 0.f;
		break;
	}

	m_Fade = EFadeState::FadeIn;
}

bool CDecalComponent::Init()
{
	if (!CPrimitiveComponent::Init())
	{
		return false;
	}

#ifdef _DEBUG
	if (m_Scene)
	{
		m_DebugMesh = (CStaticMesh*)m_Scene->GetResource()->FindMesh("CubeLinePos");
		m_DebugMaterial = m_Scene->GetResource()->FindMaterial("DebugDecal");
	}

	else
	{
		m_DebugMesh = (CStaticMesh*)CResourceManager::GetInst()->FindMesh("CubeLinePos");
		m_DebugMaterial = CResourceManager::GetInst()->FindMaterial("DebugDecal");
	}
#endif // _DEBUG

	SetWorldScale(100.f, 100.f, 100.f);

	SetMesh("CubePos");
	AddMaterial("DefaultDecal");

	return true;
}

void CDecalComponent::Update(float DeltaTime)
{
	CPrimitiveComponent::Update(DeltaTime);

	switch (m_FadeState)
	{
	case EDecalFadeState::FadeIn:
		m_FadeTime += DeltaTime;

		if (m_FadeTime >= m_FadeInTime)
		{
			m_FadeTime = m_FadeInTime;
			if (m_FinishDelete)
				m_Owner->Destroy();
		}

		m_vecMaterial[0]->SetOpacity(m_FadeTime / m_FadeInTime);
		break;
	case EDecalFadeState::FadeOut:
		m_FadeTime -= DeltaTime;

		if (m_FadeTime <= 0.f)
		{
			m_FadeTime = 0.f;
			if (m_FinishDelete)
				m_Owner->Destroy();
		}

		m_vecMaterial[0]->SetOpacity(m_FadeTime / m_FadeOutTime);
		break;
	case EDecalFadeState::FadeInOut:
		switch (m_Fade)
		{
		case EFadeState::FadeIn:
			m_FadeTime += DeltaTime;

			if (m_FadeTime >= m_FadeInTime)
			{
				m_FadeTime = 0.f;
				m_vecMaterial[0]->SetOpacity(1.f);
				m_Fade = EFadeState::Duration;
			}

			else
				m_vecMaterial[0]->SetOpacity(m_FadeTime / m_FadeInTime);

			break;
		case EFadeState::Duration:
			m_FadeTime += DeltaTime;

			if (m_FadeTime >= m_FadeOutDelayTime)
			{
				m_Fade = EFadeState::FadeOut;
				m_FadeTime = m_FadeOutTime;
			}
			break;
		case EFadeState::FadeOut:
			m_FadeTime -= DeltaTime;

			if (m_FadeTime <= 0.f)
			{
				m_FadeTime = 0.f;
				if (m_FinishDelete)
					m_Owner->Destroy();
				m_Fade = EFadeState::FadeIn;
			}

			m_vecMaterial[0]->SetOpacity(m_FadeTime / m_FadeOutTime);
			break;
		}
		break;
	}
}

void CDecalComponent::PostUpdate(float DeltaTime)
{
	CPrimitiveComponent::PostUpdate(DeltaTime);
}

void CDecalComponent::Render()
{
	CPrimitiveComponent::Render();
}

void CDecalComponent::RenderDebug()
{
	CSceneComponent::Render();
#ifdef _DEBUG
	m_DebugMaterial->SetMaterial();

	m_DebugMesh->Render();

	m_DebugMaterial->ResetMaterial();
#endif // _DEBUG
}

CDecalComponent* CDecalComponent::Clone() const
{
	return new CDecalComponent(*this);
}

void CDecalComponent::Save(FILE* File)
{
	CPrimitiveComponent::Save(File);
}

void CDecalComponent::Load(FILE* File)
{
	CPrimitiveComponent::Load(File);
}
