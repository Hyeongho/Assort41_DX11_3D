#include "PathWindow.h"
#include "PathManager.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorGUIManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Sound/Sound.h"

CPathWindow::CPathWindow()
    : m_TextureImageBox(nullptr)
    , m_FileName(nullptr)
{
}

CPathWindow::~CPathWindow()
{
}

bool CPathWindow::Init()
{
	CEditorTree<void*>* category = CreateWidget<CEditorTree<void*>>("PathWindow");
	category->SetHideName("PathWindow");
	category->SetSize(400.f, 300.f);
	category->UseDragDropSelf(true);
	category->AddItem(nullptr, "FileList");
	category->AddItem(nullptr, "...", "FileList");
	category->AddItem(nullptr, "Test", "FileList");
	
    LoadFileList();
    return true;
}

void CPathWindow::Update(float deltaTime)
{
    CEditorWindow::Update(deltaTime);
}

void CPathWindow::LoadFileList()
{
}

void CPathWindow::FileClickCallback(int index, const std::string& item)
{
}

void CPathWindow::FileDoubleClickCallback(int index, const std::string& item)
{
}

void CPathWindow::FileDragCallback(int index, const std::string& item)
{
}

void CPathWindow::FileSearch()
{
	std::string name(m_FileName->GetText());
}