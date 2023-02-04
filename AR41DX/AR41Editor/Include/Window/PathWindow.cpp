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
	/*CEditorTree<void*>* category = CreateWidget<CEditorTree<void*>>("PathWindow");
	category->SetHideName("PathWindow");
	category->SetSize(400.f, 300.f);
	category->UseDragDropSelf(true);
	category->AddItem(nullptr, "FileList");
	category->AddItem(nullptr, "...", "FileList");
	category->AddItem(nullptr, "Test", "FileList");*/

	m_Tree = CreateWidget<CEditorTree<std::string>>("Tree");

	m_Tree->SetHideName("Tree");
	
	m_Tree->SetSelectCallback<CPathWindow>(this, &CPathWindow::FileClickCallback);

    LoadFileList();

    return true;
}

void CPathWindow::Update(float deltaTime)
{
    CEditorWindow::Update(deltaTime);
}

void CPathWindow::LoadFileList()
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);

	char Path[MAX_PATH] = {};

	strcpy_s(Path, Info->PathMultibyte);

	m_Tree->AddItem("Bin", "Bin");
	m_FolderMap.insert(std::make_pair("Bin", Path));

	for (const auto& file : std::filesystem::recursive_directory_iterator(Path))
	{
		char Text[256] = {};

		sprintf_s(Text, "%s\n", file.path().filename().string().c_str());

		OutputDebugStringA(Text);

		char Name[64] = {};
		char ParentName[64] = {};
		char FullPath[MAX_PATH] = {};
		char Ext[_MAX_EXT] = {};

		strcpy_s(FullPath, file.path().generic_string().c_str());

		_splitpath_s(FullPath, nullptr, 0, ParentName, 64, Name, 64, Ext, _MAX_EXT);

		int len = (int)strlen(ParentName);

		if (len == 0)
		{
			continue;
		}

		ParentName[len - 1] = NULL;

		len = (int)strlen(ParentName);

		std::string Parent(ParentName);

		for (int i = len; i > 0; i--)
		{
			if (Parent[i] == '/' || Parent[i] == '\\')
			{
				const int j = i + 1;

				Parent = Parent.substr(j);

				break;
			}
		}

		strcpy_s(ParentName, Parent.c_str());

		if (strcmp(Ext, "") == 0)
		{
			m_Tree->AddItem("Resource", Name, ParentName);

			m_FolderMap.insert(std::make_pair(Name, FullPath));
		}

		len = 0;
	}
}

void CPathWindow::FileClickCallback(CEditorTreeItem<std::string>* Node, const std::string& item)
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