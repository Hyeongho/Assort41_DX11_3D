#include "ClientManager.h"
#include "Scene/SceneManager.h"
#include "Scene/MainSceneInfo.h"
#include "Input.h"
#include "Setting/EngineShareSetting.h"

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
    CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
    if (!CEngine::GetInst()->Init(hInst, TEXT("Client2D"), TEXT("Client2D"), IDI_ICON1,
        IDI_ICON1, 1280, 720, 1280, 720, true))
    {
        return false;
    }

    CEngineShareSetting::Setting();

    // 키 등록
    CInput::GetInst()->AddBindKey("Rotation", 'D');
    CInput::GetInst()->AddBindKey("RotationInv", 'A');

    CInput::GetInst()->AddBindKey("MoveUp", 'W');
    CInput::GetInst()->AddBindKey("MoveDown", 'S');

    CInput::GetInst()->AddBindKey("Fire", VK_SPACE);
    
    // SceneInfo 생성
    //CSceneManager::GetInst()->CreateSceneInfo<CMainSceneInfo>();
    CSceneManager::GetInst()->GetScene()->Load("Start.scn", SCENE_PATH);

    return true;
}

int CClientManager::Run()
{
    return CEngine::GetInst()->Run();
}
