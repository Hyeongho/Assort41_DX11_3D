#include "ObjectWindow.h"
#include "ComponentWindow.h"
#include "TerrainWindow.h"
#include "TransformWindow.h"
#include "UIButtonWindow.h"
#include "UIImageWindow.h"
#include "UINumberWindow.h"
#include "UIProgressBarWindow.h"
#include "UITextWindow.h"
#include "Input.h"
#include "Engine.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorSameLine.h"
#include "Component/CameraComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../GameObject\Player.h"
#include "../GameObject\Bullet.h"
#include "../GameObject\Monster.h"
#include "../GameObject\Weapon.h"

CObjectWindow::CObjectWindow()
	: m_Tree(nullptr)
	, m_WindowTree(nullptr)
	, m_CameraTransformCombo(nullptr)
	, m_CameraAxisCombo(nullptr)
{
}

CObjectWindow::~CObjectWindow()
{
}

bool CObjectWindow::Init()
{
	m_CameraTransformCombo = CreateWidget<CEditorComboBox>("CameraTransformCombo", 60.f, 30.f);
	m_CameraTransformCombo->SetHideName("CameraTransformCombo");
	m_CameraTransformCombo->AddItem("이동");
	m_CameraTransformCombo->AddItem("회전");
	m_CameraTransformCombo->Sort(false);
	m_CameraTransformCombo->SetSelectPrevViewName(true);
	m_CameraTransformCombo->SetSelectIndex(0);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_CameraAxisCombo = CreateWidget<CEditorComboBox>("CameraAxisCombo", 60.f, 30.f);
	m_CameraAxisCombo->SetHideName("CameraAxisCombo");
	m_CameraAxisCombo->AddItem("XY");
	m_CameraAxisCombo->AddItem("XZ");
	m_CameraAxisCombo->AddItem("YZ");
	m_CameraAxisCombo->AddItem("정지");
	m_CameraAxisCombo->Sort(false);
	m_CameraAxisCombo->SetSelectPrevViewName(true);
	m_CameraAxisCombo->SetSelectIndex(3);

	m_Tree = CreateWidget<CEditorTree<CGameObject*>>("ObjectTree");
	m_Tree->SetHideName("ObjectTree");
	m_Tree->SetSelectCallback<CObjectWindow>(this, &CObjectWindow::TreeCallback);
	m_Tree->SetSize(400.f, 300.f);
	m_Tree->AddItem(nullptr, "Root");
	m_WindowTree = CreateWidget<CEditorTree<CUIWindow*>>("UITree");
	m_WindowTree->SetHideName("UITree");
	m_WindowTree->SetSelectCallback<CObjectWindow>(this, &CObjectWindow::UICallback);
	m_WindowTree->SetSize(400.f, 300.f);
	m_WindowTree->AddItem(nullptr, "Canvas");

	//CInput::GetInst()->SetMouseVisible();
	AddInput(CSceneManager::GetInst()->GetScene());
	return true;
}

void CObjectWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectObject)
	{
		if (!m_SelectObject->GetActive())
		{
			m_SelectObject = nullptr;
		}
	}
	if (m_SelectWindow)
	{
		if (!m_SelectWindow->GetActive())
		{
			m_SelectWindow = nullptr;
		}
	}
}

void CObjectWindow::Clear()
{
	m_Tree->Clear();
	m_Tree->AddItem(nullptr, "Root");
	m_WindowTree->Clear();
	m_WindowTree->AddItem(nullptr, "Canvas");
}

void CObjectWindow::ClearSelect()
{
	m_SelectObject = nullptr;
	m_SelectWindow = nullptr;
}

void CObjectWindow::Delete()
{
	if (!m_SelectObject && !m_SelectWindow)
	{
		return;
	}
	if (m_SelectObject)
	{
		m_SelectObject->Destroy();
		m_Tree->DeleteItem(m_SelectObject->GetName());
	}
	else if (m_SelectWindow)
	{
		m_SelectWindow->Destroy();
		m_WindowTree->DeleteItem(m_SelectWindow->GetName());
	}
	CComponentWindow* componentWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");
	if (componentWindow)
	{
		componentWindow->Clear();
	}
	ClearSelect();
}

void CObjectWindow::Pause()
{
	if (1.f == CEngine::GetInst()->GetTimeScale())
	{
		CEngine::GetInst()->SetTimeScale(0.f);
		return;
	}
	CEngine::GetInst()->SetTimeScale(1.f);
}

void CObjectWindow::TreeCallback(CEditorTreeItem<CGameObject*>* node, const std::string& item)
{
	CComponentWindow* componentWindow =
		CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");
	if (!componentWindow)
	{
		return;
	}
	componentWindow->ClearSelect();
	componentWindow->Clear();
	m_SelectObject = node->GetCustomData();
	if (m_SelectObject)
	{
		CTransformWindow* transformWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CTransformWindow>("TransformWindow");
		CSceneComponent* root = m_SelectObject->GetRootComponent();
		if (root && transformWindow)
		{
			transformWindow->SetSelectComponent(root);
			transformWindow->SetPos(root->GetWorldPos());
			transformWindow->SetScale(root->GetWorldScale());
			transformWindow->SetRotation(root->GetWorldRot());
		}
		std::vector<HierarchyName>	vecName;
		m_SelectObject->GetAllComponentHierarchyName(vecName);
		std::string	name = m_SelectObject->GetName() + "(" + m_SelectObject->GetObjectTypeName() + ")";
		componentWindow->AddItem(nullptr, name);
		if (!vecName.empty())
		{
			std::string	parentName = name;
			name = vecName[0].Name + "(" + vecName[0].ClassName + ")";
			componentWindow->AddItem(vecName[0].Component, name, parentName);
			size_t	size = vecName.size();
			std::vector<HierarchyName>	vecName1;
			for (size_t i = 1; i < size; ++i)
			{
				parentName = vecName[i].ParentName + "(" + vecName[i].ParentClassName + ")";
				name = vecName[i].Name + "(" + vecName[i].ClassName + ")";
				if (!componentWindow->AddItem(vecName[i].Component, name, parentName))
				{
					vecName1.push_back(vecName[i]);
				}
			}
			size = vecName1.size();
			for (size_t i = 0; i < size; ++i)
			{
				parentName = vecName1[i].ParentName + "(" + vecName1[i].ParentClassName + ")";
				name = vecName1[i].Name + "(" + vecName1[i].ClassName + ")";
				componentWindow->AddItem(vecName1[i].Component, name, parentName);
			}
		}
	}
}

void CObjectWindow::UICallback(CEditorTreeItem<CUIWindow*>* node, const std::string& item)
{
	CComponentWindow* componentWindow =
		CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");
	if (!componentWindow)
	{
		return;
	}
	componentWindow->ClearSelect();
	componentWindow->Clear();
	m_SelectWindow = node->GetCustomData();
	if (m_SelectWindow)
	{
		CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		if (buttonWindow)
		{
			buttonWindow->SetSelectWindow(m_SelectWindow);
		}
		CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		if (imgWindow)
		{
			imgWindow->SetSelectWindow(m_SelectWindow);
		}
		CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		if (numberWindow)
		{
			numberWindow->SetSelectWindow(m_SelectWindow);
		}
		CUIProgressBarWindow* barWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		if (barWindow)
		{
			barWindow->SetSelectWindow(m_SelectWindow);
		}
		CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		if (textWindow)
		{
			textWindow->SetSelectWindow(m_SelectWindow);
		}
		std::vector<HierarchyWidgetName>	vecName;
		m_SelectWindow->GetAllWidgetHierarchyName(vecName);
		std::string	name = m_SelectWindow->GetWindowTypeName();
		componentWindow->AddWidget(nullptr, name);
		if (!vecName.empty())
		{
			std::string	parentName = name;
			size_t	size = vecName.size();
			for (size_t i = 0; i < size; ++i)
			{
				name = vecName[i].name + "(" + vecName[i].className + ")";
				componentWindow->AddWidget(vecName[i].widget, name, parentName);
			}
		}
	}
}

bool CObjectWindow::AddItem(CGameObject* object, const std::string& name, const std::string& parentName)
{
	return m_Tree->AddItem(object, name, parentName);
}

bool CObjectWindow::AddItem(CUIWindow* window, const std::string& name, const std::string& parentName)
{
	return m_WindowTree->AddItem(window, name, parentName);
}

void CObjectWindow::AddItemList(class CScene* scene)
{
	std::vector<HierarchyObjectName> vecName;
	scene->GetAllGameObjectHierarchyName(vecName);
	size_t	size = vecName.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddItem(vecName[i].Obj, vecName[i].Name);
	}
	std::vector<HierarchyWindowName> vecWindowName;
	scene->GetViewport()->GetAllWindowHierarchyName(vecWindowName);
	size = vecWindowName.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddItem(vecWindowName[i].window, vecWindowName[i].name);
	}
}

void CObjectWindow::AddInput(CScene* scene)
{
	CInput::GetInst()->AddBindFunction<CObjectWindow>("Del", Input_Type::Down, this, &CObjectWindow::Delete, scene);
	CInput::GetInst()->AddBindFunction<CObjectWindow>("MClick", Input_Type::Down, this, &CObjectWindow::PlaceObject, scene);
	CInput::GetInst()->AddBindFunction<CObjectWindow>("F5", Input_Type::Down, this, &CObjectWindow::Pause, scene);
	CInput::GetInst()->AddBindFunction<CObjectWindow>("UArrow", Input_Type::Push, this, &CObjectWindow::UArrow, scene);
	CInput::GetInst()->AddBindFunction<CObjectWindow>("DArrow", Input_Type::Push, this, &CObjectWindow::DArrow, scene);
	CInput::GetInst()->AddBindFunction<CObjectWindow>("LArrow", Input_Type::Push, this, &CObjectWindow::LArrow, scene);
	CInput::GetInst()->AddBindFunction<CObjectWindow>("RArrow", Input_Type::Push, this, &CObjectWindow::RArrow, scene);
	CComponentWindow* componentWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");
	if (componentWindow)
	{
		componentWindow->AddInput(scene);
	}
}

void CObjectWindow::UArrow()
{
	std::string itemName = m_CameraAxisCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionY(200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationX(-200.f * g_DeltaTime);
		}
	}
	else if (itemName == "XZ")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionZ(200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationZ(-200.f * g_DeltaTime);
		}
	}
	else if (itemName == "YZ")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionZ(200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationZ(-200.f * g_DeltaTime);
		}
	}
}

void CObjectWindow::DArrow()
{
	std::string itemName = m_CameraAxisCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionY(-200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationX(200.f * g_DeltaTime);
		}
	}
	else if (itemName == "XZ")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionZ(-200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationZ(200.f * g_DeltaTime);
		}
	}
	else if (itemName == "YZ")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionZ(-200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationZ(200.f * g_DeltaTime);
		}
	}
}

void CObjectWindow::LArrow()
{
	std::string itemName = m_CameraAxisCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionX(-200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationY(-200.f * g_DeltaTime);
		}
	}
	else if (itemName == "XZ")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionX(-200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationY(-200.f * g_DeltaTime);
		}
	}
	else if (itemName == "YZ")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionY(-200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationX(-200.f * g_DeltaTime);
		}
	}
}

void CObjectWindow::RArrow()
{
	std::string itemName = m_CameraAxisCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionX(200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationY(200.f * g_DeltaTime);
		}
	}
	else if (itemName == "XZ")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionX(200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationY(200.f * g_DeltaTime);
		}
	}
	else if (itemName == "YZ")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		itemName = m_CameraTransformCombo->GetSelectItem();
		if (itemName == "이동")
		{
			camera->AddWorldPositionY(200.f * g_DeltaTime);
		}
		else if (itemName == "회전")
		{
			camera->AddWorldRotationX(200.f * g_DeltaTime);
		}
	}
}

void CObjectWindow::PlaceObject()
{
	if (!m_SelectObject)
	{
		return;
	}
	std::string objName = m_SelectObject->GetObjectTypeName();
	if (objName == "GameObject")
	{
		return;
	}
	CGameObject* obj = nullptr;
	CreateObject(obj, objName);

	Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
	std::string itemName = m_CameraAxisCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY")
	{
		obj->SetWorldPositionX(mouseWorldPos.x);
		obj->SetWorldPositionY(mouseWorldPos.y);
	}
	else if (itemName == "XZ")
	{
		obj->SetWorldPositionX(mouseWorldPos.x);
		obj->SetWorldPositionZ(mouseWorldPos.y);
	}
	else if (itemName == "YZ")
	{
		obj->SetWorldPositionY(mouseWorldPos.x);
		obj->SetWorldPositionZ(mouseWorldPos.y);
	}
}

void CObjectWindow::CreateObject(class CGameObject* object, const std::string& name)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	if (name == "GameObject")
	{
		object = scene->CreateObject<CGameObject>(name);
	}
	else if (name == "Player")
	{
		object = scene->CreateObject<CPlayer>(name);
	}
	else if (name == "Bullet")
	{
		object = scene->CreateObject<CBullet>(name);
	}
	else if (name == "Monster")
	{
		object = scene->CreateObject<CMonster>(name);
	}
	else if (name == "Weapon")
	{
		object = scene->CreateObject<CWeapon>(name);
	}
	AddItem(object, name);
}
