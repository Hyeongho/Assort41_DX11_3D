#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CPathWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	CEditorTree<void*>* m_Tree;
	class CEditorInput* m_Path;
	std::string FileName;
	bool m_IsLoad;
	void LoadFileList(const std::string& pathName);
	void FileDCCallback(CEditorTreeItem<void*>* node, const std::string& item);
	void FileDADCallback(CEditorTreeItem<void*>* dragnode, CEditorTreeItem<void*>* dropnode, 
		const std::string& dragItem, const std::string& dropItem);
	void FileSearch();
protected:
	CPathWindow();
	virtual ~CPathWindow();
public:
	virtual bool Init();
	virtual void Update(float deltaTime);
};

