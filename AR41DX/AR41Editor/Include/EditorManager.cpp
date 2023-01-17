#include "EditorManager.h"
#include "Scene/EditorDefaultScene.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Editor/EditorGUIManager.h"
#include "GameObject\Player2D.h"
#include "GameObject\Bullet.h"
#include "GameObject\Monster.h"

#include "Window\TestWindow.h"
#include "Window\ObjectWindow.h"
#include "Window\ClassWindow.h"
#include "Window\ComponentWindow.h"
#include "Window\TransformWindow.h"
#include "Window\SceneWindow.h"
#include "Window\DetailWindow.h"
#include "Editor/EditorGUIManager.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "Input.h"
#include "CollisionManager.h"
#include "Setting/EngineShareSetting.h"
#include "Scene/EditorDefaultScene.h"
#include "Window/Animation2DWindow.h"
#include "Window/ResourceWindow.h"

CEditorManager::CEditorManager()
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

bool CEditorManager::Init(HINSTANCE hInst)
{
    CEngine::GetInst()->EnableEditor();

    if (!CEngine::GetInst()->Init(hInst, TEXT("Editor"), TEXT("Editor"), IDI_ICON1,
        IDI_ICON1, 1280, 720, 1280, 720, true, IDR_MENU1))
    {
        return false;
    }

    CSceneInfo* Info = new CEditorDefaultScene;

    CScene::AddSceneInfoCDO("EditorDefaultScene", Info);

    CEngineShareSetting::Setting();

    CEngine::SetWndProcCallback<CEditorManager>(this, &CEditorManager::WndProc);

    //CEditorGUIManager::GetInst()->CreateEditorWindow<CTestWindow>("TestWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CObjectWindow>("ObjectWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CClassWindow>("ClassWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CComponentWindow>("ComponentWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CTransformWindow>("TransformWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CSceneWindow>("SceneWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("DetailWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CAnimation2DWindow>("Animation2DWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CResourceWindow>("ResourceWindow");

    // SceneInfo »ý¼º
    CSceneManager::GetInst()->CreateSceneInfo<CEditorDefaultScene>();

    return true;
}

int CEditorManager::Run()
{
    return CEngine::GetInst()->Run();
}

bool CEditorManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_FILE_SCENE_SAVE:
            SaveScene();
            return true;
        case ID_FILE_SCENE_OPEN:
            OpenScene();
            return true;
        case ID_FILE_EXIT:
            DestroyWindow(hWnd);
            return true;
        case ID_OBJECT_EMPTY:
            CreateEmptyObject();
            return true;
        case ID_OBJECT:
            CreateObject();
            return true;
        case ID_WINDOW_TEST:            
        {
            //CEditorWindow* Window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("TestWindow");

            //if (Window)
            //    Window->Open();
        }
            return true;
        case ID_WINDOW_OBJECT:
        {
            CEditorWindow* Window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ObjectWindow");

            if (Window)
                Window->Open();
        }
            return true;
        }
        break;
    }

    return false;
}

void CEditorManager::SaveScene()
{
}

void CEditorManager::OpenScene()
{
}

void CEditorManager::CreateEmptyObject()
{
    CScene* Scene = CSceneManager::GetInst()->GetScene();

    CGameObject* EmptyObj = Scene->CreateObject<CGameObject>("GameObjectEmpty");

    CObjectWindow* Window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");

    if (Window)
    {
        Window->AddItem(EmptyObj, "GameObjectEmpty");
    }
}

void CEditorManager::CreateObject()
{
    CScene* Scene = CSceneManager::GetInst()->GetScene();

    CGameObject* Obj = nullptr;

    CClassWindow* ClassWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CClassWindow>("ClassWindow");

    std::string SelectObjectItem = ClassWindow->GetSelectObjectItem();

    if (SelectObjectItem == "")
        return;

    CObjectWindow* Window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");

    if (SelectObjectItem == "GameObject")
        Obj = Scene->CreateObject<CGameObject>(SelectObjectItem);

    else if (SelectObjectItem == "Player2D")
        Obj = Scene->CreateObject<CPlayer2D>(SelectObjectItem);

    else if (SelectObjectItem == "Bullet")
        Obj = Scene->CreateObject<CBullet>(SelectObjectItem);

    else if (SelectObjectItem == "Monster")
        Obj = Scene->CreateObject<CMonster>(SelectObjectItem);

    if (Window)
    {
        Window->AddItem(Obj, SelectObjectItem);
    }
}
