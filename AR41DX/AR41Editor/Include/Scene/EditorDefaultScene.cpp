
#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/StartSceneUI.h"
//#include "../UI/LoadingUI.h"
#include "../UI/TitleSceneUI.h"
#include "../GameObject/Player.h"
#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"

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

	//title
	//m_Owner->GetViewport()->CreateUIWindow<CTitleSceneUI>("TitleSceneUI");

	//ºñÅ°´Ï½ÃÆ¼ ¸Ê
	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	SetPlayerObject(Player);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");

	CGameObject* StaticObj = m_Owner->CreateObject<CGameObject>("StaticObj");
	CStaticMeshComponent* StaticMesh = StaticObj->CreateComponent<CStaticMeshComponent>("StaticMeshComponent");
	StaticMesh->SetMesh("BikiniBottomRoad");
	StaticMesh->SetWorldScale(0.3f, 0.3f, 0.3f);
	StaticMesh->SetWorldPosition(8650.f, -40.f, 9700.f);

	CGameObject* PariticleObj = m_Owner->CreateObject<CGameObject>("PariticleObj");
	CParticleComponent* particle = PariticleObj->CreateComponent<CParticleComponent>("ParticleComponent");

	//Á©¸®ÇÇ½¬
	return true;
}
