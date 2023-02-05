#include "PathWindow.h"
#include "MeshWindow.h"
#include "MaterialWindow.h"
#include "PathManager.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorGUIManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/Sound/Sound.h"

CPathWindow::CPathWindow()
    : m_Path(nullptr)
    , m_Tree(nullptr)
	, m_IsLoad(false)
{
}

CPathWindow::~CPathWindow()
{
}

bool CPathWindow::Init()
{
	m_Path = CreateWidget<CEditorInput>("Path", 500.f, 30.f);
	m_Path->SetHideName("Path");
	m_Tree = CreateWidget<CEditorTree<void*>>("PathWindow");
	m_Tree->SetHideName("PathWindow");
	m_Tree->SetSize(400.f, 300.f);
	m_Tree->UseDragDropSelf(true);
	m_Tree->SetDoubleClickCallback<CPathWindow>(this, &CPathWindow::FileDCCallback);
	m_Tree->SetDragAndDropCallback<CPathWindow>(this, &CPathWindow::FileDADCallback);

    LoadFileList(ROOT_PATH);
    return true;
}

void CPathWindow::Update(float deltaTime)
{
    CEditorWindow::Update(deltaTime);
	if(m_IsLoad)
	{
		m_IsLoad = false;
		LoadFileList(FileName);
	}
}

void CPathWindow::LoadFileList(const std::string& pathName)
{
//경로 저장
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);
	char	path[MAX_PATH] = {};
	if(!info)
	{
		info = CPathManager::GetInst()->FindPath(ROOT_PATH);
		strcat_s(path, m_Path->GetText());
		if(FileName=="...")
		{
			std::string fullPath = path;
			fullPath=fullPath.substr(0,fullPath.size()-1);

			size_t index = fullPath.rfind("\\");
			fullPath = fullPath.substr(0, index);
			//memset(path, 0, MAX_PATH);
			strcpy_s(path, fullPath.c_str());
		}
		else
		{
			strcat_s(path, pathName.c_str());
		}
		strcat_s(path, "\\");
	}
	else
	{
		strcpy_s(path, info->PathMultibyte);
	}
	m_Path->SetText(path);
//초기화
	m_Tree->Clear();
	m_Tree->AddItem(nullptr, "FileList");
	m_Tree->AddItem(nullptr, "...", "FileList");
//탐색
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (!strcmp(ext, ".exe") || !strcmp(ext, ".pdb") || !strcmp(ext, ".lib") || !strcmp(ext, ".ini") 
			|| !strcmp(ext, ".dll") || !strcmp(ext, ".lastcodeanalysissucceeded") || !strcmp(ext, ".ico") 
			|| !strcmp(ext, ".r18")	|| !strcmp(ext, ".r19"))
		{
			continue;
		}
		strcat_s(name, ext);
		m_Tree->AddItem(nullptr,name, "FileList");
	}
}

void CPathWindow::FileDCCallback(CEditorTreeItem<void*>* node, const std::string& item)
{
	if(item!="...")
	{
//확장자 예외처리
		size_t index = 0;
		while ((index = item.find(".", index)) != std::string::npos)
		{
			return;
		}
	}
	m_IsLoad = true;
	FileName = item;
}

void CPathWindow::FileDADCallback(CEditorTreeItem<void*>* dragnode, CEditorTreeItem<void*>* dropnode,
	const std::string& dragItem, const std::string& dropItem)
{
	if(ImGui::IsWindowHovered("MeshWindow"))
	{
		//경로 합치기
		//	strcpy_s(fullPath, file.path().generic_string().c_str());
		
		//확장자로 bne msh구별
		char	ext[_MAX_EXT] = {};
		_splitpath_s(dragItem.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
		//tchar 변환
		TCHAR* t_filename = new TCHAR[dragItem.size() + 1];
		t_filename[dragItem.size()] = 0;
		std::copy(dragItem.begin(), dragItem.end(), t_filename);
		CMeshWindow* meshWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMeshWindow>("MeshWindow");
		if (meshWindow)
		{
			if(!strcmp(ext, ".msh"))
			{
				meshWindow->MeshChangeCallback(t_filename);
			}
			else if (!strcmp(ext, ".fbx"))
			{
				meshWindow->MeshChangeCallback(t_filename);
			}
			else if (!strcmp(ext, ".bne"))
			{
				meshWindow->SkeletonChangeCallback(t_filename);
			}
		}
		SAFE_DELETE_ARRAY(t_filename);
	}
	else if (ImGui::IsWindowHovered("MaterialWindow"))
	{
		TCHAR* t_filename = new TCHAR[dragItem.size() + 1];
		t_filename[dragItem.size()] = 0;
		std::copy(dragItem.begin(), dragItem.end(), t_filename);
		CMaterialWindow* materialWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMaterialWindow>("MaterialWindow");
		if (materialWindow)
		{
			materialWindow->ImgChangeCallback(dragItem,t_filename);
		}
		SAFE_DELETE_ARRAY(t_filename);
	}
}

void CPathWindow::FileSearch()
{
	std::string name(m_Path->GetText());
	//if (name == "")
	//{
	//	m_FontList->Clear();
	//	LoadFileList();
	//	return;
	//}
	//size_t size = m_FontList->GetSize();
	//std::vector<std::string> vecName;
	//std::string checkedName;
	//for (size_t i = 0; i < size; ++i)
	//{
	//	checkedName = m_FontList->CheckPartitionItem(i, name);
	//	if (checkedName != "")
	//	{
	//		vecName.push_back(checkedName);
	//	}
	//}
	//size_t vecSize = vecName.size();
	//if (vecSize != 0)
	//{
	//	m_FontList->Clear();
	//	for (size_t i = 0; i < vecSize; ++i)
	//	{
	//		m_FontList->AddItem(vecName[i]);
	//	}
	//}
}