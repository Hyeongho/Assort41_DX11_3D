#pragma once

#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"
#include "GameObject/GameObject.h"
#include "UI/UIWindow.h"

class CObjectWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CObjectWindow();
	virtual ~CObjectWindow();

private:
	CEditorTree<CGameObject*>* m_Tree;
	CSharedPtr<CGameObject>	m_SelectObject;
	CEditorTree<CUIWindow*>* m_WindowTree;
	CSharedPtr<CUIWindow>	m_SelectWindow;

public:
	class CGameObject* GetSelectObject()	const
	{
		return m_SelectObject;
	}
	class CUIWindow* GetSelectWindow()	const
	{
		return m_SelectWindow;
	}
public:
	bool AddItem(class CGameObject* Object, const std::string& Name, const std::string& ParentName = "Root");
	bool AddItem(class CUIWindow* window, const std::string& name, const std::string& parentName = "Canvas");
	void AddItemList(class CScene* scene);
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void Clear();
	void ClearSelect();
	void Delete();

private:
	void TreeCallback(CEditorTreeItem<CGameObject*>* Node, const std::string& Item);
	void UICallback(CEditorTreeItem<CUIWindow*>* node, const std::string& item);
};

