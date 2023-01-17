#pragma once

#include "Editor/EditorWindow.h"
#include "Editor/EditorTree.h"

class CTestWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CTestWindow();
	virtual ~CTestWindow();

private:
	class CEditorText* m_Text;
	char	m_AddText[1024];
	int		m_AddIndex;
	int		m_AddCount;
	float	m_AddTime;

	class CEditorInput* m_Input;
	class CEditorListBox* m_List;
	class CEditorComboBox* m_Combo;
	CEditorTree<int>* m_Tree;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void TestButtonCallback();
	void InputCallback();
	void ListCallback(int SelectIndex, const std::string& Item);
	void TreeCallback(CEditorTreeItem<int>* Node, const std::string& Item);
};

