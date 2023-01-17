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

public:
	class CComponent* GetSelectComponent()	const
	{
		return m_SelectComponent;
	}

public:
	bool AddItem(class CComponent* Component, const std::string& Name, const std::string& ParentName = "Root");
	void Clear();
	void ClearSelect();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void TreeCallback(CEditorTreeItem<class CComponent*>* Node, const std::string& Item);
};

