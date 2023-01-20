
#include "SceneManager.h"
#include "SceneInfo.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "../Animation/Animation2D.h"
#include "../UI/UIWindow.h"
#include "../UI/UIWidget.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_Scene(nullptr),
	m_NextScene(nullptr)
{
	CScene::CreateCDO();
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_Scene);

	{
		auto	iter = CScene::m_mapSceneInfoCDO.begin();
		auto	iterEnd = CScene::m_mapSceneInfoCDO.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}

	{
		auto	iter = CGameObject::m_mapObjectCDO.begin();
		auto	iterEnd = CGameObject::m_mapObjectCDO.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}

	{
		auto	iter = CComponent::m_mapComponent.begin();
		auto	iterEnd = CComponent::m_mapComponent.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}

	{
		auto	iter = CAnimation2D::m_mapAnimationCDO.begin();
		auto	iterEnd = CAnimation2D::m_mapAnimationCDO.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}

	{
		auto	iter = CUIWindow::m_mapUIWindowCDO.begin();
		auto	iterEnd = CUIWindow::m_mapUIWindowCDO.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}

	{
		auto	iter = CUIWidget::m_mapUIWidgetCDO.begin();
		auto	iterEnd = CUIWidget::m_mapUIWidgetCDO.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

	m_Scene->Init();

	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	if (!m_Scene->m_Start)
	{
		m_Scene->Start();
	}

	m_Scene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	if (!m_Scene->m_Start)
	{
		m_Scene->Start();
	}

	m_Scene->PostUpdate(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::Collision(float DeltaTime)
{
	m_Scene->Collision(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::ChangeScene()
{
	if (m_NextScene)
	{
		if (m_NextScene->m_Change)
		{
			SAFE_DELETE(m_Scene);
			m_Scene = m_NextScene;
			m_NextScene = nullptr;

			m_Scene->GetSceneInfo()->SceneChangeComplete();

			return true;
		}
	}

	return false;
}

void CSceneManager::CreateNextScene(bool AutoChange)
{
	SAFE_DELETE(m_NextScene);

	m_NextScene = new CScene;

	m_NextScene->m_Change = AutoChange;
}

void CSceneManager::ChangeNextScene()
{
	m_NextScene->m_Change = true;
}
