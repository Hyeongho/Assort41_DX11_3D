
#include "ComponentWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"
#include "Component/Component.h"
#include "TransformWindow.h"
#include "Editor/EditorGUIManager.h"
#include "Component/SceneComponent.h"
#include "DetailWindow.h"
#include "Input.h"
#include "UI/UIWidget.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UINumber.h"
#include "UI/UIProgressBar.h"
#include "UI/UIText.h"
#include "UIButtonWindow.h"
#include "UIImageWindow.h"
#include "UINumberWindow.h"
#include "UIProgressBarWindow.h"
#include "UITextWindow.h"

CComponentWindow::CComponentWindow()
	: m_Tree(nullptr)
	, m_WidgetTree(nullptr)
{
}

CComponentWindow::~CComponentWindow()
{
}

bool CComponentWindow::AddItem(CComponent* Component, const std::string& Name, const std::string& ParentName)
{
	return m_Tree->AddItem(Component, Name, ParentName);
}

bool CComponentWindow::AddWidget(CUIWidget* widget, const std::string& name, const std::string& parentName)
{
	return m_WidgetTree->AddItem(widget, name, parentName);
}

void CComponentWindow::Clear()
{
	m_Tree->Clear();
	m_WidgetTree->Clear();
}

void CComponentWindow::ClearSelect()
{
	m_SelectComponent = nullptr;
	m_SelectWidget = nullptr;
}

void CComponentWindow::ChangePos()
{
	if (!m_SelectComponent && !m_SelectWidget)
	{
		return;
	}
	Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
	if (m_SelectComponent)
	{
		CSceneComponent* component = (CSceneComponent*)m_SelectComponent.Get();
		component->SetWorldPosition(mouseWorldPos);
	}
	else if (m_SelectWidget)
	{
		m_SelectWidget->SetPos(mouseWorldPos);
	}
}

bool CComponentWindow::Init()
{
	m_Tree = CreateWidget<CEditorTree<CComponent*>>("ComponentTree");

	m_Tree->SetHideName("ComponentTree");

	m_Tree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::TreeCallback);

	m_Tree->SetSize(400.f, 300.f);

	m_WidgetTree = CreateWidget<CEditorTree<CUIWidget*>>("WidgetTree");
	m_WidgetTree->SetHideName("WidgetTree");
	m_WidgetTree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::WidgetCallback);
	m_WidgetTree->SetSize(400.f, 300.f);
	return true;
}

void CComponentWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);

	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
			m_SelectComponent = nullptr;
	}
	if (m_SelectWidget)
	{
		if (!m_SelectWidget->GetActive())
		{
			m_SelectWidget = nullptr;
		}
	}
}

void CComponentWindow::TreeCallback(CEditorTreeItem<class CComponent*>* Node, const std::string& Item)
{
	char	Text[256] = {};

	sprintf_s(Text, "%s\n", Item.c_str());

	OutputDebugStringA(Text);

	m_SelectComponent = Node->GetCustomData();

	CTransformWindow* TransformWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CTransformWindow>("TransformWindow");
	CDetailWindow* DetailWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CDetailWindow>("DetailWindow");
	 
	DetailWindow->SetSelectComponent((CSceneComponent*)m_SelectComponent.Get());

	if (m_SelectComponent)
	{
		CSceneComponent* Component = (CSceneComponent*)m_SelectComponent.Get();

		TransformWindow->SetSelectComponent(Component);

		if (Component->GetParent())
		{
			TransformWindow->SetPos(Component->GetRelativePos());
			TransformWindow->SetScale(Component->GetRelativeScale());
			TransformWindow->SetRotation(Component->GetRelativeRot());
		}

		else
		{
			TransformWindow->SetPos(Component->GetWorldPos());
			TransformWindow->SetScale(Component->GetWorldScale());
			TransformWindow->SetRotation(Component->GetWorldRot());
		}
	}
}

void CComponentWindow::WidgetCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item)
{
	m_SelectWidget = node->GetCustomData();
	if (!m_SelectWidget)
	{
		return;
	}
	if (m_SelectWidget->GetWidgetTypeName() == "UIButton")
	{
		CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		if (buttonWindow)
		{
			buttonWindow->SetSelectWidget((CUIButton*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIImage")
	{
		CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		if (imgWindow)
		{
			imgWindow->SetSelectWidget((CUIImage*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UINumber")
	{
		CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		if (numberWindow)
		{
			numberWindow->SetSelectWidget((CUINumber*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIProgressBar")
	{
		CUIProgressBarWindow* barWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		if (barWindow)
		{
			barWindow->SetSelectWidget((CUIProgressBar*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIText")
	{
		CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		if (textWindow)
		{
			textWindow->SetSelectWidget((CUIText*)m_SelectWidget.Get());
		}
	}
}
