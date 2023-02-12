#include "FBXWindow.h"
#include "Engine.h"
#include "PathManager.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Resource/ResourceManager.h"

CFBXWindow::CFBXWindow()
	: m_FBX_CB(nullptr)
	, m_Sequence_CB(nullptr)
{
}

CFBXWindow::~CFBXWindow()
{
}

bool CFBXWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("FBX 파일 변환");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("변환", 50.f, 30.f);
	button->SetClickCallback<CFBXWindow>(this, &CFBXWindow::FBXConvertCallback);

	m_Sequence_CB = CreateWidget<CEditorCheckBox>("시퀸스 변환");
	line = CreateWidget<CEditorSameLine>("Line");
	m_FBX_CB = CreateWidget<CEditorCheckBox>("애니메이션 매쉬");
	return true;
}

void CFBXWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CFBXWindow::FBXConvert(const TCHAR* path)
{
	char name[256];
	char ext[256];
	WideCharToMultiByte(CP_ACP, 0, path, _MAX_EXT, name, _MAX_EXT, NULL, NULL);
	_splitpath_s(name, nullptr, 0, nullptr, 0, name, _MAX_EXT, ext, 256);
	if(m_Sequence_CB->GetCheck())
	{
		CResourceManager::GetInst()->LoadAnimationSequenceFullPath(name, path);
	}
	else
	{
		unsigned char check = m_FBX_CB->GetCheck() ? 3U : 2U;
		CResourceManager::GetInst()->LoadMeshFullPath(nullptr, (MeshType)check, name, path);
	}
}

void CFBXWindow::FBXConvertCallback()
{
	OPENFILENAME	ofn = {};
	TCHAR	fullPath[MAX_PATH] = {};
	TCHAR	filter[] = TEXT("모든 파일\0*.*\0FBX\0*.fbx");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fullPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = CPathManager::GetInst()->FindPath(MESH_PATH)->Path;
	if (GetOpenFileName(&ofn) != 0)
	{
		FBXConvert(fullPath);
	}
}
