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
	: m_FBXName(nullptr)
	, m_FBX_CB(nullptr)
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
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(3.f);
	m_FBXName = CreateWidget<CEditorInput>("FBXName", 80.f, 30.f);
	m_FBXName->SetHideName("FBXName");
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-1.5f);
	CEditorButton* button = CreateWidget<CEditorButton>("변환", 50.f, 30.f);
	button->SetClickCallback<CFBXWindow>(this, &CFBXWindow::FBXConvertCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-1.5f);
	button = CreateWidget<CEditorButton>("시퀸스 변환", 80.f, 30.f);
	button->SetClickCallback<CFBXWindow>(this, &CFBXWindow::SequenceConvertCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_FBX_CB = CreateWidget<CEditorCheckBox>("애니메이션 매쉬");
	return true;
}

void CFBXWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
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
		TCHAR	wTexName[256] = {};
		TCHAR	wExt[256] = {};
		_wsplitpath_s(fullPath, 0, 0, 0, 0, wTexName, 256, wExt, 256);
		wcscat_s(wTexName, wExt);
		unsigned char check = m_FBX_CB->GetCheck() ? 3U: 2U;
		CResourceManager::GetInst()->LoadMesh(nullptr, (MeshType)check, m_FBXName->GetText(), wTexName);
	}
}

void CFBXWindow::SequenceConvertCallback()
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
		TCHAR	wTexName[256] = {};
		TCHAR	wExt[256] = {};
		_wsplitpath_s(fullPath, 0, 0, 0, 0, wTexName, 256, wExt, 256);
		wcscat_s(wTexName, wExt);
		CResourceManager::GetInst()->LoadAnimationSequence(m_FBXName->GetText(), wTexName, MESH_PATH);
	}
}
