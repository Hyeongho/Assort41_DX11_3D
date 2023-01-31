#pragma once
#include "Editor\EditorWindow.h"

class CPathWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	CEditorTree<class CComponent*>* m_Tree;
	class CEditorImage* m_TextureImageBox;
	class CEditorInput* m_FileName;
	std::string fullPath;
	void LoadFileList();
	void FileClickCallback(int index, const std::string& item);
	void FileDoubleClickCallback(int index, const std::string& item);
	void FileDragCallback(int index, const std::string& item);
	void FileSearch();
protected:
	CPathWindow();
	virtual ~CPathWindow();
public:
	virtual bool Init();
	virtual void Update(float deltaTime);
};

