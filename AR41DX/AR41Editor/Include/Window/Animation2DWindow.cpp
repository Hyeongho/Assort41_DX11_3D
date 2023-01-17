#include "Animation2DWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorGroup.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../GameObject\Player2D.h"
#include "../GameObject\Bullet.h"
#include "../GameObject\Monster.h"
#include "ObjectWindow.h"
#include "ComponentWindow.h"
#include "Editor/EditorGUIManager.h"
#include "ResourceWindow.h"
#include "Engine.h"
#include "Resource/ResourceManager.h"
#include "Resource/Animation/AnimationSequence2D.h"

#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"

#include "DetailWindow.h"
#include "DetailWindow/SpriteComponentWidgetList.h"

CAnimation2DWindow::CAnimation2DWindow()
{
}

CAnimation2DWindow::~CAnimation2DWindow()
{
}

bool CAnimation2DWindow::Init()
{
	CEditorLabel* Label = CreateWidget<CEditorLabel>("Animation2DClass");

	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(150.f, 30.f);

	CEditorSameLine* Line = CreateWidget<CEditorSameLine>("Line");

	CEditorButton* Anim2DCreateButton = CreateWidget<CEditorButton>("Anim2DCreate");
	Anim2DCreateButton->SetSize(140.f, 30.f);
	Anim2DCreateButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::CreateAnimation2D);


	//Line = CreateWidget<CEditorSameLine>("Line");
	//Line->SetOffsetX(330.f);

	m_Animation2DClassList = CreateWidget<CEditorListBox>("Animation2DClassListBox");

	m_Animation2DClassList->SetHideName("Animation2DClassListBox");

	m_Animation2DClassList->SetSize(300.f, 300.f);
	m_Animation2DClassList->SetPageItemCount(6);
	m_Animation2DClassList->SetSelectCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::Animation2DSelectCallback);

	//Line = CreateWidget<CEditorSameLine>("Line");
	//Line->SetOffsetX(330.f);

	Label = CreateWidget<CEditorLabel>("Sequence2D");

	Label->SetColor(255, 0, 0, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(150.f, 30.f);

	m_Animation2DSequenceList = CreateWidget<CEditorListBox>("Animation2DSequenceListBox");

	m_Animation2DSequenceList->SetHideName("Animation2DSequenceListBox");

	m_Animation2DSequenceList->SetSize(300.f, 300.f);
	m_Animation2DSequenceList->SetPageItemCount(6);
	m_Animation2DSequenceList->SetSelectCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::SequenceSelectCallback);

	Line = CreateWidget<CEditorSameLine>("Line");

	CEditorGroup* Anim2DSequenceGroup = CreateWidget<CEditorGroup>("Anim2DGroup");

	Label = Anim2DSequenceGroup->CreateWidget<CEditorLabel>("Texture");

	Label->SetColor(0, 0, 255, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(150.f, 30.f);

	m_Sequence2DImage = Anim2DSequenceGroup->CreateWidget<CEditorImage>("Sequence2DTexture");

	m_Sequence2DImage->SetSize(100.f, 100.f);

	Line = Anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");

	CEditorButton* TextureSelectButton = Anim2DSequenceGroup->CreateWidget<CEditorButton>("TextureSelect");

	TextureSelectButton->SetSize(150.f, 30.f);
	TextureSelectButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::SelectTextureButton);

	Label = Anim2DSequenceGroup->CreateWidget<CEditorLabel>("Frame");

	Label->SetColor(0, 0, 255, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(150.f, 30.f);

	Line = Anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");

	CEditorButton* CreateSquenceButton = Anim2DSequenceGroup->CreateWidget<CEditorButton>("CreateSequence");

	CreateSquenceButton->SetSize(150.f, 30.f);
	CreateSquenceButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::CreateAnimationSequence2D);



	Label = Anim2DSequenceGroup->CreateWidget<CEditorLabel>("Start");

	Label->SetColor(0, 0, 255, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(80.f, 30.f);

	Line = Anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");

	m_StartX = Anim2DSequenceGroup->CreateWidget<CEditorInput>("StartX");

	m_StartX->SetHideName("StartX");
	m_StartX->SetSize(100.f, 30.f);
	m_StartX->SetInputType(EImGuiInputType::Float);

	Line = Anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");

	m_StartY = Anim2DSequenceGroup->CreateWidget<CEditorInput>("StartY");

	m_StartY->SetHideName("StartY");
	m_StartY->SetSize(100.f, 30.f);
	m_StartY->SetInputType(EImGuiInputType::Float);

	Label = Anim2DSequenceGroup->CreateWidget<CEditorLabel>("End");

	Label->SetColor(0, 0, 255, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(80.f, 30.f);

	Line = Anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");

	m_EndX = Anim2DSequenceGroup->CreateWidget<CEditorInput>("EndX");

	m_EndX->SetHideName("EndX");
	m_EndX->SetSize(100.f, 30.f);
	m_EndX->SetInputType(EImGuiInputType::Float);

	Line = Anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");

	m_EndY = Anim2DSequenceGroup->CreateWidget<CEditorInput>("EndY");

	m_EndY->SetHideName("EndY");
	m_EndY->SetSize(100.f, 30.f);
	m_EndY->SetInputType(EImGuiInputType::Float);




	Label = Anim2DSequenceGroup->CreateWidget<CEditorLabel>("Count");

	Label->SetColor(0, 0, 255, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(80.f, 30.f);

	Line = Anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");

	m_Count = Anim2DSequenceGroup->CreateWidget<CEditorInput>("Count");

	m_Count->SetHideName("Count");
	m_Count->SetSize(150.f, 30.f);
	m_Count->SetInputType(EImGuiInputType::Int);

	Label = Anim2DSequenceGroup->CreateWidget<CEditorLabel>("FrameList");

	Label->SetColor(0, 0, 255, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSize(80.f, 30.f);

	m_SequenceFrameList = Anim2DSequenceGroup->CreateWidget<CEditorListBox>("SequenceFrameListBox");

	m_SequenceFrameList->SetHideName("SequenceFrameListBox");

	m_SequenceFrameList->SetSize(100.f, 200.f);
	m_SequenceFrameList->SetPageItemCount(6);
	m_SequenceFrameList->SetSelectCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::FrameSelectCallback);


	CEditorButton*	AddFrameButton = Anim2DSequenceGroup->CreateWidget<CEditorButton>("AddFrame");

	AddFrameButton->SetSize(150.f, 30.f);
	AddFrameButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::AddFrameClick);

	Line = Anim2DSequenceGroup->CreateWidget<CEditorSameLine>("Line");


	AddFrameButton = Anim2DSequenceGroup->CreateWidget<CEditorButton>("AddAllFrame");

	AddFrameButton->SetSize(150.f, 30.f);
	AddFrameButton->SetClickCallback<CAnimation2DWindow>(this, &CAnimation2DWindow::AddAllFrameClick);



	LoadAnimation2DClassList();
	LoadAnimation2DSequenceList();

    return true;
}

void CAnimation2DWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);
}

void CAnimation2DWindow::LoadAnimation2DClassList()
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);

	char	Path[MAX_PATH] = {};

	strcpy_s(Path, Info->PathMultibyte);

	int	Length = (int)strlen(Path);

	// Root Path에서 Bin\을 지워준다.
	for (int i = Length - 2; i >= 0; --i)
	{
		if (Path[i] == '/' || Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	char	Directory[MAX_PATH] = {};

	strcpy_s(Directory, Path);

	// Editor의 GameObject폴더에 있는 파일을 읽어온다.
	strcat_s(Directory, "Include/Animation2D/");

	for (const auto& file : std::filesystem::recursive_directory_iterator(Directory))
	{
		char	Name[256] = {};
		char	FullPath[MAX_PATH] = {};
		char	Ext[_MAX_EXT] = {};
		char	FolderName[12] = {};

		strcpy_s(FullPath, file.path().generic_string().c_str());

		int	Length = (int)strlen(FullPath);

		if (Length >= 11)
		{
			for (int i = Length - 12; i >= 0; --i)
			{
				memcpy(FolderName, &FullPath[i], 11);

				if (strcmp(FolderName, "/Animation/") == 0)
				{
					strcpy_s(Name, &FullPath[i + 11]);
					break;
				}
			}
		}

		_splitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

		if (strcmp(Ext, ".cpp") == 0)
			continue;

		Length = (int)strlen(Name);

		Name[Length - 1] = 0;
		Name[Length - 2] = 0;

		m_Animation2DClassList->AddItem(Name);
	}

	Length = (int)strlen(Path);

	// Root Path에서 Bin\을 지워준다.
	for (int i = Length - 2; i >= 0; --i)
	{
		if (Path[i] == '/' || Path[i] == '\\')
		{
			memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	m_Animation2DClassList->AddItem("Animation2D");
}

void CAnimation2DWindow::LoadAnimation2DSequenceList()
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);

	char	Path[MAX_PATH] = {};

	strcpy_s(Path, Info->PathMultibyte);

	char	Directory[MAX_PATH] = {};

	strcpy_s(Directory, Path);

	// Editor의 GameObject폴더에 있는 파일을 읽어온다.
	strcat_s(Directory, "Animation2D/");

	for (const auto& file : std::filesystem::recursive_directory_iterator(Directory))
	{
		char	Name[256] = {};
		char	FullPath[MAX_PATH] = {};
		char	Ext[_MAX_EXT] = {};
		char	FolderName[14] = {};

		strcpy_s(FullPath, file.path().generic_string().c_str());

		int	Length = (int)strlen(FullPath);

		if (Length >= 13)
		{
			for (int i = Length - 14; i >= 0; --i)
			{
				memcpy(FolderName, &FullPath[i], 13);

				if (strcmp(FolderName, "/Animation2D/") == 0)
				{
					strcpy_s(Name, &FullPath[i + 13]);
					break;
				}
			}
		}

		_splitpath_s(FullPath, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

		_strupr_s(Ext);

		if (strcmp(Ext, ".SQC") != 0)
			continue;

		Length = (int)strlen(Name);
		Name[Length - 1] = 0;
		Name[Length - 2] = 0;
		Name[Length - 3] = 0;
		Name[Length - 4] = 0;

		m_Animation2DSequenceList->AddItem(Name);

		CResourceManager::GetInst()->LoadSequence2D(Name, FullPath);
	}
}

void CAnimation2DWindow::CreateAnimation2D()
{
}

void CAnimation2DWindow::SelectTextureButton()
{
	CResourceWindow* ResourceWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CResourceWindow>("ResourceWindow");

	m_SelectTexture = ResourceWindow->GetSelectTexture();

	m_Sequence2DImage->SetTexture(m_SelectTexture);
}

void CAnimation2DWindow::CreateAnimationSequence2D()
{
	// 이미지가 없거나 프레임 정보가 없을 경우 생성 금지.
	if (!m_SelectTexture || m_vecFrame.empty())
		return;

	// 파일 다이얼로그

	OPENFILENAME	ofn = {};

	TCHAR	FullPath[MAX_PATH] = {};

	TCHAR	Filter[] = TEXT("모든 파일\0*.*\0SQC\0*.sqc");

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	ofn.lpstrFilter = Filter;
	ofn.lpstrFile = FullPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION2D_PATH)->Path;

	if (GetSaveFileName(&ofn) != 0)
	{
		char	MultibytePath[MAX_PATH] = {};

		int	Length = (int)WideCharToMultiByte(CP_ACP, 0, FullPath, -1,
			0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FullPath, -1, MultibytePath, Length, 0, 0);

		char	FileName[_MAX_EXT] = {};
		_splitpath_s(MultibytePath, nullptr, 0, nullptr, 0, FileName, _MAX_EXT, nullptr, 0);

		// Sequence 객체를 생성하고 프레임 정보를 넣어준다.
		if (!CResourceManager::GetInst()->CreateAnimationSequence2D(FileName, m_SelectTexture))
			return;

		int	Count = (int)m_vecFrame.size();

		for (int i = 0; i < Count; ++i)
		{
			CResourceManager::GetInst()->AddAnimationSequence2DFrame(
				FileName, m_vecFrame[i].Start, m_vecFrame[i].End);
		}

		CResourceManager::GetInst()->SaveSequence2D(FileName, MultibytePath);


		Length = (int)strlen(MultibytePath);
		// /Animation2D/
		char	FolderName[14] = {};


		if (Length >= 14)
		{
			for (int i = Length - 15; i >= 0; --i)
			{
				memcpy(FolderName, &MultibytePath[i], 13);

				if (strcmp(FolderName, "/Animation2D/") == 0)
				{
					memset(FileName, 0, _MAX_EXT);
					strcpy_s(FileName, &MultibytePath[i + 13]);
					break;
				}
			}
		}


		// 추가된 시퀀스 이름을 리스트에 추가한다.
		m_Animation2DSequenceList->AddItem(FileName);

		//CAnimationSequence2D* Sequence =
			


	}



}

void CAnimation2DWindow::AddFrameClick()
{
	Animation2DFrameData	Data;

	Data.Start.x = m_StartX->GetFloat();
	Data.Start.y = m_StartY->GetFloat();

	Data.End.x = m_EndX->GetFloat();
	Data.End.y = m_EndY->GetFloat();

	int	Number = (int)m_vecFrame.size();

	char	NumberString[4] = {};
	_itoa_s(Number, NumberString, 10);

	m_SequenceFrameList->AddItem(NumberString);

	m_vecFrame.push_back(Data);
}

void CAnimation2DWindow::AddAllFrameClick()
{
	m_vecFrame.clear();

	m_SequenceFrameList->Clear();

	int	Count = m_Count->GetInt();

	for (int i = 0; i < Count; ++i)
	{
		Animation2DFrameData	Data;

		Data.Start.x = m_StartX->GetFloat();
		Data.Start.y = m_StartY->GetFloat();

		Data.End.x = m_EndX->GetFloat();
		Data.End.y = m_EndY->GetFloat();

		char	NumberString[4] = {};
		_itoa_s(i, NumberString, 10);

		m_SequenceFrameList->AddItem(NumberString);

		m_vecFrame.push_back(Data);
	}
}

void CAnimation2DWindow::Animation2DSelectCallback(int Index, 
	const std::string& Item)
{
	m_Animation2DSelectName = Item;
}

void CAnimation2DWindow::SequenceSelectCallback(int Index, const std::string& Item)
{
	m_Animation2DSequenceSelectName = Item;

	CDetailWindow* DetailWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CDetailWindow>("DetailWindow");

	if (DetailWindow->GetSelectComponent()->GetComponentTypeName() !=
		"SpriteComponent")
		return;

	CSpriteComponentWidgetList* SpriteWidget =
		(CSpriteComponentWidgetList*)DetailWindow->GetComponentWidgetList("SpriteComponent");

	SpriteWidget->SetSelectAnimationSequence2DName(Item);
}

void CAnimation2DWindow::FrameSelectCallback(int Index, const std::string& Item)
{
	m_StartX->SetFloat(m_vecFrame[Index].Start.x);
	m_StartY->SetFloat(m_vecFrame[Index].Start.y);
	m_EndX->SetFloat(m_vecFrame[Index].End.x);
	m_EndY->SetFloat(m_vecFrame[Index].End.y);
}
