
#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/StartSceneUI.h"
//#include "../UI/LoadingUI.h"
#include "Component/LightComponent.h"

CEditorDefaultScene::CEditorDefaultScene()
{
	m_ClassTypeName = "EditorDefaultScene";
}

CEditorDefaultScene::~CEditorDefaultScene()
{
}

bool CEditorDefaultScene::Init()
{
	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent*  GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(45.f, 90.f, 0.f);
	//m_Owner->GetViewport()->CreateUIWindow<CStartSceneUI>("StartUI");
	//m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");

	return true;
}
