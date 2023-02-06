#pragma once

#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CPathWindow
	: public CEditorWindow
{
	friend class CEditorGUIManager;

	CEditorTree<void*>* m_Tree;
	class CEditorInput* m_Path;
	std::string FileName;
	bool m_IsLoad;
	void LoadFileList(const std::string& pathName);
	void FileDCCallback(CEditorTreeItem<void*>* node, const std::string& item);
	void FileDADCallback(CEditorTreeItem<void*>* dragnode, CEditorTreeItem<void*>* dropnode, 
		const std::string& dragItem, const std::string& dropItem);

protected:
	CPathWindow();
	virtual ~CPathWindow();

private:
	void LoadFileList();
	void FileClickCallback(CEditorTreeItem<std::string>* Node, const std::string& item);
	void FileDoubleClickCallback(int index, const std::string& item);
	void FileDragCallback(int index, const std::string& item);

	void FileSearch();

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
};

