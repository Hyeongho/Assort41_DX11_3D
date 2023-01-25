#pragma once

#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CComponentWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CComponentWindow();
	virtual ~CComponentWindow();

private:
	CEditorTree<class CComponent*>* m_Tree;
	CSharedPtr<class CComponent>	m_SelectComponent;

	CEditorTree<class CUIWidget*>* m_WidgetTree;
	CSharedPtr<class CUIWidget>	m_SelectWidget;
public:
	class CComponent* GetSelectComponent()	const
	{
		return m_SelectComponent;
	}
	class CUIWidget* GetSelectWidget()	const
	{
		return m_SelectWidget;
	}
public:
	bool AddItem(class CComponent* Component, const std::string& Name, const std::string& ParentName = "Root");
	bool AddWidget(class CUIWidget* widget, const std::string& name, const std::string& parentName = "Canvas");
	void Clear();
	void ClearSelect();
	void ChangePos();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void TreeCallback(CEditorTreeItem<class CComponent*>* Node, const std::string& Item);
	void WidgetCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item);
};

