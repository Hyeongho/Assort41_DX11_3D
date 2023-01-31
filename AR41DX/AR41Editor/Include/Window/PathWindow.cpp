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
	//m_ImageName = category->CreateWidget<CEditorInput>("Sprite", "SpriteName");
	//m_Image = category->CreateWidget<CEditorImage>("Sprite", "SpriteImage");
	//m_Image->SetIsFixed(true);
	//category->CreateWidget<CEditorSameLine>("Sprite", "Line");
	//CEditorButton* button = category->CreateWidget<CEditorButton>("Sprite", "SelectTexture");
	//button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::SelectTextureButtonClick);
	//m_AnimationClass = category->CreateWidget<CEditorInput>("Animation", "AnimationClass", 100.f, 30.f);
	//m_AnimationClass->ReadOnly(true);
	//m_AnimationClass->SetText("None");
	//m_AnimationClass->SetSize(150.f, 30.f);
	//category->CreateWidget<CEditorSameLine>("Animation", "Line");
	//button = category->CreateWidget<CEditorButton>("Animation", "CreateAnimation");
	//button->SetSize(150.f, 30.f);
	//button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::CreateAnimationButtonClick);
	//m_AnimationList = category->CreateWidget<CEditorListBox>("Animation", "AnimationList");
	//m_AnimationList->SetHideName("AnimationList");
	//m_AnimationList->SetSize(150.f, 300.f);
	//m_AnimationList->SetPageItemCount(6);
	//m_AnimationList->SetSelectCallback<CSpriteComponentWidgetList>
	//	(this, &CSpriteComponentWidgetList::AnimationSelectCallback);
	//category->CreateWidget<CEditorSameLine>("Animation", "Line");
	//CEditorGroup* animDataGroup = category->CreateWidget<CEditorGroup>("Animation", "Animation2DData");
	//m_Animation2DDataName = animDataGroup->CreateWidget<CEditorInput>("Animation2DDataName", 150.f, 30.f);
	//m_Animation2DDataName->SetText("None");
	//m_SequenceName = animDataGroup->CreateWidget<CEditorInput>("SequenceName", 150.f, 30.f);
	//m_SequenceName->ReadOnly(true);
	//m_SequenceName->SetText("None");
	//m_PlayTime = animDataGroup->CreateWidget<CEditorInput>("PlayTime", 100.f, 30.f);
	//m_PlayTime->SetInputType(EImGuiInputType::Float);
	//m_PlayScale = animDataGroup->CreateWidget<CEditorInput>("PlayScale", 100.f, 30.f);
	//m_PlayScale->SetInputType(EImGuiInputType::Float);
	//m_LoopCheck = animDataGroup->CreateWidget<CEditorCheckBox>("Loop", 100.f, 30.f);
	//m_ReverseCheck = animDataGroup->CreateWidget<CEditorCheckBox>("Reverse", 100.f, 30.f);
	//button = animDataGroup->CreateWidget<CEditorButton>("AddAnimation2D", 150.f, 30.f);
	//button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::AddAnimation2DData);
	//animDataGroup->CreateWidget<CEditorSameLine>("Line");
	//button = animDataGroup->CreateWidget<CEditorButton>("DeleteAnimation2D", 150.f, 30.f);
	//button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::DeleteAnimation2DData);
	//button = animDataGroup->CreateWidget<CEditorButton>("ModifyAnimation2D", 150.f, 30.f);
	//button->SetClickCallback<CSpriteComponentWidgetList>(this, &CSpriteComponentWidgetList::ModifyAnimation2DData);
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