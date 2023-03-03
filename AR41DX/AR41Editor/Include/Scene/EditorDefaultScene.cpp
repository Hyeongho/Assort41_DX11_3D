
#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/StartSceneUI.h"
//#include "../UI/LoadingUI.h"
#include "../UI/TitleSceneUI.h"
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
	GlobalLightComponent->SetRelativeRotation(0, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	//m_Owner->GetViewport()->CreateUIWindow<CStartSceneUI>("StartUI");
	//m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");

	//tile
	//m_Owner->GetViewport()->CreateUIWindow<CTitleSceneUI>("TitleSceneUI");
	return true;
}
