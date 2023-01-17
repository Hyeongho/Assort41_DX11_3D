
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

CComponentWindow::CComponentWindow()
{
}

CComponentWindow::~CComponentWindow()
{
}

bool CComponentWindow::AddItem(CComponent* Component, const std::string& Name, const std::string& ParentName)
{
	return m_Tree->AddItem(Component, Name, ParentName);
}

void CComponentWindow::Clear()
{
	m_Tree->Clear();
}

void CComponentWindow::ClearSelect()
{
	m_SelectComponent = nullptr;
}

bool CComponentWindow::Init()
{
	m_Tree = CreateWidget<CEditorTree<CComponent*>>("ComponentTree");

	m_Tree->SetHideName("ComponentTree");

	m_Tree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::TreeCallback);

	m_Tree->SetSize(400.f, 300.f);

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
