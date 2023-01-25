
#include "ObjectWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"
#include "ComponentWindow.h"
#include "TransformWindow.h"
#include "Editor/EditorGUIManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CObjectWindow::CObjectWindow()
	: m_Tree(nullptr)
	, m_WindowTree(nullptr)
{
}

CObjectWindow::~CObjectWindow()
{
}

bool CObjectWindow::AddItem(CGameObject* Object, const std::string& Name, const std::string& ParentName)
{
	return m_Tree->AddItem(Object, Name, ParentName);
}

bool CObjectWindow::AddItem(CUIWindow* window, const std::string& name, const std::string& parentName)
{
	return m_WindowTree->AddItem(window, name, parentName);
}

void CObjectWindow::AddItemList()
{
	std::vector<HierarchyObjectName> vecName;
	CSceneManager::GetInst()->GetScene()->GetAllGameObjectHierarchyName(vecName);
	size_t	size = vecName.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddItem(vecName[i].Obj, vecName[i].Name);
	}
	std::vector<HierarchyWindowName> vecWindowName;
	CSceneManager::GetInst()->GetScene()->GetViewport()->GetAllWindowHierarchyName(vecWindowName);
	size = vecWindowName.size();
	for (size_t i = 0; i < size; ++i)
	{
		AddItem(vecWindowName[i].window, vecWindowName[i].name);
	}
}

bool CObjectWindow::Init()
{
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
	return true;
}

void CObjectWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);

	if (m_SelectObject)
	{
		if (!m_SelectObject->GetActive())
			m_SelectObject = nullptr;
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
	ClearSelect();
}

void CObjectWindow::TreeCallback(CEditorTreeItem<CGameObject*>* Node, const std::string& Item)
{
	char	Text[256] = {};

	sprintf_s(Text, "%s\n", Item.c_str());

	OutputDebugStringA(Text);

	CComponentWindow* ComponentWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");

	ComponentWindow->ClearSelect();
	ComponentWindow->Clear();

	// 해당 게임오브젝트가 가지고 있는 모든 컴포넌트의 이름을 얻어온다.
	CGameObject* Obj = Node->GetCustomData();

	m_SelectObject = Obj;

	if (Obj)
	{
		CTransformWindow* TransformWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CTransformWindow>("TransformWindow");

		CSceneComponent* Root = Obj->GetRootComponent();

		if (Root)
		{
			TransformWindow->SetSelectComponent(Root);
			TransformWindow->SetPos(Root->GetWorldPos());
			TransformWindow->SetScale(Root->GetWorldScale());
			TransformWindow->SetRotation(Root->GetWorldRot());
		}

		std::vector<HierarchyName>	vecName;

		Obj->GetAllComponentHierarchyName(vecName);

		std::string	Name = Obj->GetName() + "(" + Obj->GetObjectTypeName() + ")";

		ComponentWindow->AddItem(nullptr, Name);

		if (!vecName.empty())
		{
			std::string	ParentName = Name;

			Name = vecName[0].Name + "(" + vecName[0].ClassName + ")";

			ComponentWindow->AddItem(vecName[0].Component, Name, ParentName);

			size_t	Size = vecName.size();

			std::vector<HierarchyName>	vecName1;

			for (size_t i = 1; i < Size; ++i)
			{
				ParentName = vecName[i].ParentName + "(" + vecName[i].ParentClassName + ")";

				Name = vecName[i].Name + "(" + vecName[i].ClassName + ")";

				if (!ComponentWindow->AddItem(vecName[i].Component, Name, ParentName))
					vecName1.push_back(vecName[i]);
			}

			Size = vecName1.size();

			for (size_t i = 0; i < Size; ++i)
			{
				ParentName = vecName1[i].ParentName + "(" + vecName1[i].ParentClassName + ")";

				Name = vecName1[i].Name + "(" + vecName1[i].ClassName + ")";

				ComponentWindow->AddItem(vecName1[i].Component, Name, ParentName);
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
		//CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		//if (buttonWindow)
		//{
		//	buttonWindow->SetSelectWindow(m_SelectWindow);
		//}
		//CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		//if (imgWindow)
		//{
		//	imgWindow->SetSelectWindow(m_SelectWindow);
		//}
		//CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		//if (numberWindow)
		//{
		//	numberWindow->SetSelectWindow(m_SelectWindow);
		//}
		//CUIProgressBarWindow* barWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		//if (barWindow)
		//{
		//	barWindow->SetSelectWindow(m_SelectWindow);
		//}
		//CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		//if (textWindow)
		//{
		//	textWindow->SetSelectWindow(m_SelectWindow);
		//}
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
