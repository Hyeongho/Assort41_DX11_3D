#include "EditorManager.h"
#include "Scene/EditorDefaultScene.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Editor/EditorGUIManager.h"
#include "GameObject\Player.h"
#include "GameObject\Bullet.h"
#include "GameObject\Monster.h"
#include "GameObject\Weapon.h"

#include "Window/StatusWindow.h"
#include "Window\ObjectWindow.h"
#include "Window\ClassWindow.h"
#include "Window\ComponentWindow.h"
#include "Window\TransformWindow.h"
#include "Window\SceneWindow.h"
#include "Window\FBXWindow.h"
#include "Window/PathWindow.h"
#include "Window/MaterialWindow.h"
#include "Window/MeshWindow.h"
#include "Editor/EditorGUIManager.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "Input.h"
#include "CollisionManager.h"
#include "Setting/EngineShareSetting.h"
#include "Scene/EditorDefaultScene.h"
#include "Scene/BikiniCitySceneInfo.h"
#include "Scene/TitleSceneInfo.h"
#include "Scene/TestScene.h"
#include "Scene/JellyfishFieldSceneInfo.h"
#include "Scene/JellyfishFieldSceneInfo.h"
#include "Scene/JellyfishField1SceneInfo.h"
#include "Scene/JellyfishField2SceneInfo.h"
#include "Scene/JellyfishField3SceneInfo.h"
#include "Scene/JellyfishField4SceneInfo.h"
#include "Scene/JellyfishField5SceneInfo.h"
#include "Scene/KingJellyfishSceneInfo.h"

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

    if (!CEngine::GetInst()->Init(hInst, TEXT("Editor"), TEXT("Editor"), IDI_ICON2,
        IDI_ICON2, 1280, 720, 1280, 720, true, IDR_MENU1))
    {
        return false;
    }


    CSceneInfo* Info = new CEditorDefaultScene;
    //CSceneInfo* Info = new CTitleSceneInfo;
    //CSceneInfo* Info = new CBikiniCitySceneInfo;
    //CSceneInfo* Info = new CJellyfishFieldSceneInfo;
    //CSceneInfo* Info = new CKingJellyfishSceneInfo;

    CScene::AddSceneInfoCDO("EditorDefaultScene", Info);
    //CScene::AddSceneInfoCDO("TitleSceneInfo", Info);
    //CScene::AddSceneInfoCDO("BikiniCitySceneInfo", Info);
    //CScene::AddSceneInfoCDO("JellyfishFieldSceneInfo", Info);
    //CScene::AddSceneInfoCDO("KingJellyfishSceneInfo", Info);

    CEngineShareSetting::Setting();

    CEngine::SetWndProcCallback<CEditorManager>(this, &CEditorManager::WndProc);

    CEditorGUIManager::GetInst()->CreateEditorWindow<CObjectWindow>("ObjectWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CClassWindow>("ClassWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CComponentWindow>("ComponentWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CTransformWindow>("TransformWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CSceneWindow>("SceneWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CStatusWindow>("StatusWindow");
    //CEditorGUIManager::GetInst()->CreateEditorWindow<CFBXWindow>("FBXWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CPathWindow>("PathWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CMaterialWindow>("MaterialWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CMeshWindow>("MeshWindow");

    // SceneInfo 
    //CSceneManager::GetInst()->CreateSceneInfo<CTestScene>();
    CSceneManager::GetInst()->CreateSceneInfo<CEditorDefaultScene>();
    //CSceneManager::GetInst()->CreateSceneInfo<CTitleSceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CBikiniCitySceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CJellyfishFieldSceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CJellyfishField1SceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CJellyfishField2SceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CJellyfishField3SceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CJellyfishField4SceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CJellyfishField5SceneInfo>();
    //CSceneManager::GetInst()->CreateSceneInfo<CKingJellyfishSceneInfo>();

    CObjectWindow* objectWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
    if (objectWindow)
    {
        objectWindow->AddItemList(CSceneManager::GetInst()->GetScene());
    }
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

    else if (SelectObjectItem == "Player")
        Obj = Scene->CreateObject<CPlayer>(SelectObjectItem);

    else if (SelectObjectItem == "Bullet")
        Obj = Scene->CreateObject<CBullet>(SelectObjectItem);

    else if (SelectObjectItem == "Monster")
        Obj = Scene->CreateObject<CMonster>(SelectObjectItem);

    else if (SelectObjectItem == "Weapon")
        Obj = Scene->CreateObject<CWeapon>(SelectObjectItem);

    if (Window)
    {
        Window->AddItem(Obj, SelectObjectItem);
    }
}
