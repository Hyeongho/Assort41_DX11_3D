#include "UIProgressBarWindow.h"
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
#include "UI/UIProgressBar.h"
#include "UI/UIWindow.h"

CUIProgressBarWindow::CUIProgressBarWindow()
	: m_BarTypeCB(nullptr)
	, m_BarTextureTypeCB(nullptr)
	, m_PlayTime(nullptr)
	, m_PlayScale(nullptr)
	, m_BarTypeIndex(0)
	, m_BarTextureTypeIndex(0)
	, m_Count(0)
	, m_Slider(nullptr)
	, m_MinInput(nullptr)
	, m_MaxInput(nullptr)
	, m_ValueInput(nullptr)
	, m_PosX(nullptr)
	, m_PosY(nullptr)
	, m_SizeX(nullptr)
	, m_SizeY(nullptr)
{
}

CUIProgressBarWindow::~CUIProgressBarWindow()
{
}

void CUIProgressBarWindow::SetSelectWidget(CUIProgressBar* widget)
{
	m_SelectWidget = widget;
	m_Slider->SetPosition(0, m_SelectWidget->GetTint().x * 255.f);
	m_Slider->SetPosition(1, m_SelectWidget->GetTint().y * 255.f);
	m_Slider->SetPosition(2, m_SelectWidget->GetTint().z * 255.f);
	m_Slider->SetPosition(3, m_SelectWidget->GetOpacity() * 255.f);
	m_BarTypeIndex = (int)m_SelectWidget->GetBarDir();
	std::string stateName = m_BarTypeCB->GetItem(m_BarTypeIndex);
	m_BarTypeCB->SetPrevViewName(stateName);
	m_PlayTime->SetFloat(m_SelectWidget->GetTextureInfo(0).PlayTime);
	m_PlayScale->SetFloat(m_SelectWidget->GetTextureInfo(0).PlayScale);
	m_PosX->SetFloat(m_SelectWidget->GetPos().x);
	m_PosY->SetFloat(m_SelectWidget->GetPos().y);
	m_SizeX->SetFloat(m_SelectWidget->GetSize().x);
	m_SizeY->SetFloat(m_SelectWidget->GetSize().y);
	m_MinInput->SetFloat(m_SelectWidget->GetMinValue());
	m_MaxInput->SetFloat(m_SelectWidget->GetMaxValue());
	m_ValueInput->SetFloat(m_SelectWidget->GetValue());
}

void CUIProgressBarWindow::SetSelectWindow(CUIWindow* window)
{
	m_SelectWindow = window;
}

bool CUIProgressBarWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("UI ProgressBar");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("����", 50.f, 30.f);
	button->SetClickCallback<CUIProgressBarWindow>(this, &CUIProgressBarWindow::BarCreateCallback);
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_BarTypeCB = CreateWidget<CEditorComboBox>("BarTypeCB");
	m_BarTypeCB->SetHideName("BarTypeCB");
	m_BarTypeCB->SetPrevViewName("RightToLeft");
	m_BarTypeCB->AddItem("RightToLeft");
	m_BarTypeCB->AddItem("LeftToRight");
	m_BarTypeCB->AddItem("TopToBottom");
	m_BarTypeCB->AddItem("BottomToTop");
	m_BarTypeCB->SetSelectPrevViewName(true);
	m_BarTypeCB->SetSelectCallback<CUIProgressBarWindow>(this, &CUIProgressBarWindow::BarTypeCBCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.5f);
	button = CreateWidget<CEditorButton>("����", 50.f, 30.f);
	button->SetClickCallback<CUIProgressBarWindow>(this, &CUIProgressBarWindow::BarChangeCallback);
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_BarTextureTypeCB = CreateWidget<CEditorComboBox>("BarTextureTypeCB");
	m_BarTextureTypeCB->SetHideName("BarTextureTypeCB");
	m_BarTextureTypeCB->SetPrevViewName("Back");
	m_BarTextureTypeCB->AddItem("Back");
	m_BarTextureTypeCB->AddItem("Bar");
	m_BarTextureTypeCB->SetSelectPrevViewName(true);
	m_BarTextureTypeCB->SetSelectCallback<CUIProgressBarWindow>(this, &CUIProgressBarWindow::BarTextureTypeCBCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(-2.5f);
	button = CreateWidget<CEditorButton>("�̹��� ����", 80.f, 30.f);
	button->SetClickCallback<CUIProgressBarWindow>(this, &CUIProgressBarWindow::TextureChangeCallback);
	m_Slider = CreateWidget<CEditorSlider>("Slider");
	m_Slider->SetText("����");
	m_PlayTime = CreateWidget<CEditorInput>("����ð�", 80.f, 30.f);
	m_PlayTime->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_PlayScale = CreateWidget<CEditorInput>("�������", 80.f, 30.f);
	m_PlayScale->SetInputType(EImGuiInputType::Float);
	m_PosX = CreateWidget<CEditorInput>("X ��ġ", 80.f, 30.f);
	m_PosX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(23.f);
	m_PosY = CreateWidget<CEditorInput>("Y ��ġ", 80.f, 30.f);
	m_PosY->SetInputType(EImGuiInputType::Float);
	m_SizeX = CreateWidget<CEditorInput>("X ũ��", 80.f, 30.f);
	m_SizeX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(23.f);
	m_SizeY = CreateWidget<CEditorInput>("Y ũ��", 80.f, 30.f);
	m_SizeY->SetInputType(EImGuiInputType::Float);
	m_MinInput = CreateWidget<CEditorInput>("�ּҺ���", 80.f, 30.f);
	m_MinInput->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(-5.f);
	m_MaxInput = CreateWidget<CEditorInput>("�ִ����", 80.f, 30.f);
	m_MaxInput->SetInputType(EImGuiInputType::Float);
	m_ValueInput = CreateWidget<CEditorInput>("ä�����", 80.f, 30.f);
	m_ValueInput->SetInputType(EImGuiInputType::Float);
	return true;
}

void CUIProgressBarWindow::Update(float deltaTime)
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

void CUIProgressBarWindow::BarTypeCBCallback(int index, const std::string& item)
{
	m_BarTypeIndex = index;
}

void CUIProgressBarWindow::BarTextureTypeCBCallback(int index, const std::string& item)
{
	m_BarTextureTypeIndex = index;
	if(!m_SelectWidget)
	{
		return;
	}
	m_PlayTime->SetFloat(m_SelectWidget->GetTextureInfo(index).PlayTime);
	m_PlayScale->SetFloat(m_SelectWidget->GetTextureInfo(index).PlayScale);
}

void CUIProgressBarWindow::BarCreateCallback()
{
	if (!m_SelectWindow)
	{
		return;
	}
	++m_Count;
	std::string name = "ProgressBar_" + std::to_string(m_Count);
	CUIProgressBar* progressBar = m_SelectWindow->CreateWidget<CUIProgressBar>(name);
	Vector2 pos = m_SelectWindow->GetSize() * 0.5f;
	progressBar->SetPos(pos.x, pos.y);
	progressBar->SetSize(200.f, 30.f);
	progressBar->SetImageTint(EProgressBarTextureType::Back, 100, 100, 100, 255);
    progressBar->SetTexture(EProgressBarTextureType::Bar, name, TEXT("HPBar.png"));
}

void CUIProgressBarWindow::BarChangeCallback()
{
	if (!m_SelectWidget)
	{
		return;
	}
	m_SelectWidget->SetImageTint((EProgressBarTextureType)m_BarTextureTypeIndex,
		(unsigned char)m_Slider->GetPosition(0), (unsigned char)m_Slider->GetPosition(1),
		(unsigned char)m_Slider->GetPosition(2), (unsigned char)m_Slider->GetPosition(3));
	m_SelectWidget->SetOpacity(m_Slider->GetPosition(3) / 255.f);
	m_SelectWidget->SetPlayTime((EProgressBarTextureType)m_BarTextureTypeIndex, m_PlayTime->GetFloat());
	m_SelectWidget->SetPlayScale((EProgressBarTextureType)m_BarTextureTypeIndex, m_PlayScale->GetFloat());
	m_SelectWidget->SetProgressBarMin(m_MinInput->GetFloat());
	m_SelectWidget->SetProgressBarMax(m_MaxInput->GetFloat());
	m_SelectWidget->SetValue(m_ValueInput->GetFloat());
	m_SelectWidget->SetPos(m_PosX->GetFloat(), m_PosY->GetFloat());
	m_SelectWidget->SetSize(m_SizeX->GetFloat(), m_SizeY->GetFloat());
	m_SelectWidget->SetBarDir((EProgressBarDir)m_BarTypeIndex);
}

void CUIProgressBarWindow::TextureChangeCallback()
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
		m_SelectWidget->SetTextureFullPath((EProgressBarTextureType)m_BarTextureTypeIndex, texName, fullPath);
	}
}