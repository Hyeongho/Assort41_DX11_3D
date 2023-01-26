#include "UIButtonWindow.h"
#include "PathManager.h"
#include "Engine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorComboBox.h"
#include "UI/UIButton.h"
#include "UI/UIWindow.h"
#include "Resource/ResourceManager.h"
#include "Resource/Sound/Sound.h"


CUIButtonWindow::CUIButtonWindow()
	: m_ButtonEnableCheck(nullptr)
	, m_PlayTime(nullptr)
	, m_PlayScale(nullptr)
	, m_ButtonStateIndex(0)
	, m_ButtonEventStateIndex(0)
	, m_Count(0)
	, m_Slider(nullptr)
	, m_ButtonStateCB(nullptr)
	, m_ButtonEventStateCB(nullptr)
	, m_PosX(nullptr)
	, m_PosY(nullptr)
	, m_SizeX(nullptr)
	, m_SizeY(nullptr)
{
}

CUIButtonWindow::~CUIButtonWindow()
{
}

void CUIButtonWindow::SetSelectWidget(CUIButton* widget)
{
	m_SelectWidget = widget;
	m_Slider->SetPosition(0, m_SelectWidget->GetTint().x*255.f);
	m_Slider->SetPosition(1, m_SelectWidget->GetTint().y * 255.f);
	m_Slider->SetPosition(2, m_SelectWidget->GetTint().z * 255.f);
	m_Slider->SetPosition(3, m_SelectWidget->GetOpacity() * 255.f);
	m_ButtonStateIndex = (int)m_SelectWidget->GetState();
	std::string stateName = m_ButtonStateCB->GetItem(m_ButtonStateIndex);
	m_ButtonStateCB->SetPrevViewName(stateName);
	m_ButtonEnableCheck->SetCheck(m_SelectWidget->GetButtonEnable());
	m_PlayTime->SetFloat(m_SelectWidget->GetTextureInfo(m_ButtonStateIndex).PlayTime);
	m_PlayScale->SetFloat(m_SelectWidget->GetTextureInfo(m_ButtonStateIndex).PlayScale);
	m_PosX->SetFloat(m_SelectWidget->GetPos().x);
	m_PosY->SetFloat(m_SelectWidget->GetPos().y);
	m_SizeX->SetFloat(m_SelectWidget->GetSize().x);
	m_SizeY->SetFloat(m_SelectWidget->GetSize().y);
}

void CUIButtonWindow::SetSelectWindow(CUIWindow* window)
{
	m_SelectWindow = window;
}

bool CUIButtonWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("UI ��ư");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("����", 50.f, 30.f);
	button->SetClickCallback<CUIButtonWindow>(this, &CUIButtonWindow::ButtonCreateCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ButtonEnableCheck = CreateWidget<CEditorCheckBox>("Ȱ��ȭ ��ư");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_ButtonStateCB = CreateWidget<CEditorComboBox>("ButtonStateComboBox");
	m_ButtonStateCB->SetHideName("ButtonStateComboBox");
	m_ButtonStateCB->SetPrevViewName("Normal");
	m_ButtonStateCB->AddItem("Normal");
	m_ButtonStateCB->AddItem("Hovered");
	m_ButtonStateCB->AddItem("Click");
	m_ButtonStateCB->AddItem("Disable");
	m_ButtonStateCB->SetSelectPrevViewName(true);
	m_ButtonStateCB->SetSelectCallback<CUIButtonWindow>(this, &CUIButtonWindow::StateCBCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.5f);
	button = CreateWidget<CEditorButton>("����", 50.f, 30.f);
	button->SetClickCallback<CUIButtonWindow>(this, &CUIButtonWindow::ButtonChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.5f);
	button = CreateWidget<CEditorButton>("�̹��� ����", 80.f, 30.f);
	button->SetClickCallback<CUIButtonWindow>(this, &CUIButtonWindow::TextureChangeCallback);
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_ButtonEventStateCB = CreateWidget<CEditorComboBox>("ButtonEventStateCB");
	m_ButtonEventStateCB->SetHideName("ButtonEventStateCB");
	m_ButtonEventStateCB->SetPrevViewName("Hovered");
	m_ButtonEventStateCB->AddItem("Hovered");
	m_ButtonEventStateCB->AddItem("Click");
	m_ButtonEventStateCB->SetSelectPrevViewName(true);
	m_ButtonEventStateCB->SetSelectCallback<CUIButtonWindow>(this, &CUIButtonWindow::EventStateCBCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.5f);
	button = CreateWidget<CEditorButton>("���� ����", 80.f, 30.f);
	button->SetClickCallback<CUIButtonWindow>(this, &CUIButtonWindow::SoundChangeCallback);
	m_Slider = CreateWidget<CEditorSlider>("Slider");
	m_Slider->SetText("����");
	m_PlayTime = CreateWidget<CEditorInput>("����ð�", 80.f, 30.f);
	m_PlayTime->SetInputType(EImGuiInputType::Float);
	m_PlayScale = CreateWidget<CEditorInput>("�������", 80.f, 30.f);
	m_PlayScale->SetInputType(EImGuiInputType::Float);
	m_PosX = CreateWidget<CEditorInput>("X ��ġ", 80.f, 30.f);
	m_PosX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_PosY = CreateWidget<CEditorInput>("Y ��ġ", 80.f, 30.f);
	m_PosY->SetInputType(EImGuiInputType::Float);
	m_SizeX = CreateWidget<CEditorInput>("X ũ��", 80.f, 30.f);
	m_SizeX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_SizeY = CreateWidget<CEditorInput>("Y ũ��", 80.f, 30.f);
	m_SizeY->SetInputType(EImGuiInputType::Float);
	return true;
}

void CUIButtonWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectWidget)
	{
		if (!m_SelectWidget->GetActive())
		{
			m_SelectWidget = nullptr;
		}
	}
	if (m_SelectWindow)
	{
		if (!m_SelectWindow->GetActive())
		{
			m_SelectWindow = nullptr;
		}
	}
}

void CUIButtonWindow::StateCBCallback(int index, const std::string& item)
{
	m_ButtonStateIndex = index;
}

void CUIButtonWindow::EventStateCBCallback(int index, const std::string& item)
{
	m_ButtonEventStateIndex = index;
}

void CUIButtonWindow::ButtonCreateCallback()
{
	if(!m_SelectWindow)
	{
		return;
	}
	++m_Count;
	std::string name = "Button_"+ std::to_string(m_Count);
	CUIButton* m_Button = m_SelectWindow->CreateWidget<CUIButton>(name);
	Vector2 pos=m_SelectWindow->GetSize()*0.5f;
	m_Button->SetPos(pos.x, pos.y);
	m_Button->SetSize(100.f, 100.f);
	m_Button->SetTexture(EButtonState::Normal, name, TEXT("DefaultUI.png"));
	m_Button->SetTexture(EButtonState::Hovered, name, TEXT("DefaultUI.png"));
	m_Button->SetTexture(EButtonState::Click, name, TEXT("DefaultUI.png"));
	m_Button->SetTexture(EButtonState::Disable, name, TEXT("DefaultUI.png"));
}

void CUIButtonWindow::ButtonChangeCallback()
{
	if (!m_SelectWidget)
	{
		return;
	}
	m_SelectWidget->SetImageTint((EButtonState)m_ButtonStateIndex, 
		(unsigned char)m_Slider->GetPosition(0), (unsigned char)m_Slider->GetPosition(1), 
		(unsigned char)m_Slider->GetPosition(2), (unsigned char)m_Slider->GetPosition(3));
	m_SelectWidget->SetOpacity(m_Slider->GetPosition(3)/255.f);
	m_SelectWidget->ButtonEnable(m_ButtonEnableCheck->GetCheck());
	m_SelectWidget->SetPlayTime((EButtonState)m_ButtonStateIndex, m_PlayTime->GetFloat());
	m_SelectWidget->SetPlayScale((EButtonState)m_ButtonStateIndex, m_PlayScale->GetFloat());
	m_SelectWidget->SetPos(m_PosX->GetFloat(), m_PosY->GetFloat());
	m_SelectWidget->SetSize(m_SizeX->GetFloat(), m_SizeY->GetFloat());
}

void CUIButtonWindow::TextureChangeCallback()
{
	if (!m_SelectWidget)
	{
		return;
	}
	OPENFILENAME	ofn = {};
	TCHAR	fullPath[MAX_PATH] = {};
	TCHAR	filter[] = TEXT("��� ����\0*.*\0PNG\0*.png\0JPG\0*.jpg\0BMP\0*.bmp\0GIF\0*.gif");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fullPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;
	if (GetOpenFileName(&ofn) != 0)
	{
		TCHAR	wTexName[256] = {};
		_wsplitpath_s(fullPath, 0, 0, 0, 0, wTexName, 256, 0, 0);
		char	texName[256] = {};
		int length = (int)WideCharToMultiByte(CP_ACP, 0, wTexName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, wTexName, -1, texName, length, nullptr, nullptr);
		m_SelectWidget->SetTextureFullPath((EButtonState)m_ButtonStateIndex, texName, fullPath);
	}
}

void CUIButtonWindow::SoundChangeCallback()
{
	if (!m_SelectWidget)
	{
		return;
	}
	OPENFILENAME	ofn = {};
	TCHAR	wFullPath[MAX_PATH] = {};
	TCHAR	filter[] = TEXT("��� ����\0*.*\0WAV\0*.wav\0MP3\0*.mp3\0OGG\0*.ogg");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = wFullPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = CPathManager::GetInst()->FindPath(SOUND_PATH)->Path;
	if (GetOpenFileName(&ofn) != 0)
	{
		TCHAR	wSoundName[256] = {};
		TCHAR	wSoundExt[256] = {};
		_wsplitpath_s(wFullPath, 0, 0, 0, 0, wSoundName, 256, wSoundExt, 256);
		char	soundName[256] = {};
		int length = (int)WideCharToMultiByte(CP_ACP, 0, wSoundName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, wSoundName, -1, soundName, length, nullptr, nullptr);

		_tcscat_s(wSoundName, wSoundExt);
		char	fileName[256] = {};
		length = (int)WideCharToMultiByte(CP_ACP, 0, wSoundName, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_ACP, 0, wSoundName, -1, fileName, length, nullptr, nullptr);

		m_SelectWidget->SetSound((EButtonEventState)m_ButtonEventStateIndex, "UI", soundName, false, fileName);
	}

}