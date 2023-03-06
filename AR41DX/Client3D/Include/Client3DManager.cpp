#include "Client3DManager.h"
#include "Scene/SceneManager.h"
#include "Scene/MainSceneInfo.h"
#include "Scene/JellyfishFieldSceneInfo.h"
#include "Input.h"
#include "Setting/EngineShareSetting.h"

#include "Scene/TestScene.h"

CClient3DManager::CClient3DManager()
{
}

CClient3DManager::~CClient3DManager()
{
    CEngine::DestroyInst();
}

bool CClient3DManager::Init(HINSTANCE hInst)
{
    if (!CEngine::GetInst()->Init(hInst, TEXT("Client3D"), TEXT("Client3D"), IDI_ICON1,
        IDI_ICON1, 1280, 720, 1280, 720, true))
    {
        return false;
    }

    CEngineShareSetting::Setting();

    // SceneInfo »ý¼º
    CSceneManager::GetInst()->CreateSceneInfo<CTestScene>();
    //CSceneManager::GetInst()->CreateSceneInfo<CMainSceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CJellyfishFieldSceneInfo>();
    //CSceneManager::GetInst()->GetScene()->Load("Start.scn", SCENE_PATH);

    return true;
}

int CClient3DManager::Run()
{
    return CEngine::GetInst()->Run();
}

