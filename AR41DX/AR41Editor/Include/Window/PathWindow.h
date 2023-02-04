#pragma once

#include "Editor\EditorWindow.h"

class CPathWindow
	: public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CPathWindow();
	virtual ~CPathWindow();

private:
	CEditorTree<std::string>* m_Tree;
	class CEditorImage* m_TextureImageBox;
	class CEditorInput* m_FileName;

	std::unordered_map<std::string, std::string> m_FolderMap;

	std::string fullPath;

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

