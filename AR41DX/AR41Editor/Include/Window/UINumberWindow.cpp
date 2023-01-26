#include "UINumberWindow.h"
#include "PathManager.h"
#include "Engine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorSlider.h"
#include "UI/UINumber.h"
#include "UI/UIWindow.h"

CUINumberWindow::CUINumberWindow()
	: m_NumberInput(nullptr)
	, m_SpaceInput(nullptr)
	, m_PlayTime(nullptr)
	, m_PlayScale(nullptr)
	, m_Count(0)
	, m_Slider(nullptr)
	, m_PosX(nullptr)
	, m_PosY(nullptr)
	, m_SizeX(nullptr)
	, m_SizeY(nullptr)
{
}

CUINumberWindow::~CUINumberWindow()
{
}

void CUINumberWindow::SetSelectWidget(CUINumber* widget)
{
	m_SelectWidget = widget;
	m_Slider->SetPosition(0, m_SelectWidget->GetTint().x * 255.f);
	m_Slider->SetPosition(1, m_SelectWidget->GetTint().y * 255.f);
	m_Slider->SetPosition(2, m_SelectWidget->GetTint().z * 255.f);
	m_Slider->SetPosition(3, m_SelectWidget->GetOpacity() * 255.f);
	m_PlayTime->SetFloat(m_SelectWidget->GetTextureInfo().PlayTime);
	m_PlayScale->SetFloat(m_SelectWidget->GetTextureInfo().PlayScale);
	m_PosX->SetFloat(m_SelectWidget->GetPos().x);
	m_PosY->SetFloat(m_SelectWidget->GetPos().y);
	m_SizeX->SetFloat(m_SelectWidget->GetSize().x);
	m_SizeY->SetFloat(m_SelectWidget->GetSize().y);
	m_NumberInput->SetInt(m_SelectWidget->GetNumber());
	m_SpaceInput->SetFloat(m_SelectWidget->GetSpace());
}

void CUINumberWindow::SetSelectWindow(CUIWindow* window)
{
	m_SelectWindow = window;
}

bool CUINumberWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("UI Number");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("생성", 50.f, 30.f);
	button->SetClickCallback<CUINumberWindow>(this, &CUINumberWindow::NumberCreateCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("설정", 50.f, 30.f);
	button->SetClickCallback<CUINumberWindow>(this, &CUINumberWindow::NumberChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("이미지 변경", 80.f, 30.f);
	button->SetClickCallback<CUINumberWindow>(this, &CUINumberWindow::TextureChangeCallback);
	m_Slider = CreateWidget<CEditorSlider>("Slider");
	m_Slider->SetText("색상");
	m_PlayTime = CreateWidget<CEditorInput>("재생시간", 80.f, 30.f);
	m_PlayTime->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_PlayScale = CreateWidget<CEditorInput>("재생비율", 80.f, 30.f);
	m_PlayScale->SetInputType(EImGuiInputType::Float);
	m_NumberInput = CreateWidget<CEditorInput>("숫자", 120.f, 30.f);
	m_NumberInput->SetInputType(EImGuiInputType::Int);
	m_SpaceInput = CreateWidget<CEditorInput>("자간", 80.f, 30.f);
	m_SpaceInput->SetInputType(EImGuiInputType::Float);
	m_PosX = CreateWidget<CEditorInput>("X 위치", 80.f, 30.f);
	m_PosX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_PosY = CreateWidget<CEditorInput>("Y 위치", 80.f, 30.f);
	m_PosY->SetInputType(EImGuiInputType::Float);
	m_SizeX = CreateWidget<CEditorInput>("X 크기", 80.f, 30.f);
	m_SizeX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_SizeY = CreateWidget<CEditorInput>("Y 크기", 80.f, 30.f);
	m_SizeY->SetInputType(EImGuiInputType::Float);
	return true;
}

void CUINumberWindow::Update(float deltaTime)
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

void CUINumberWindow::NumberCreateCallback()
{
	//생성할때 크기 지정해줘야 한다
	if (!m_SelectWindow)
	{
		return;
	}
	++m_Count;
	std::string name = "Number_" + std::to_string(m_Count);
	CUINumber* number = m_SelectWindow->CreateWidget<CUINumber>(name);
	Vector2 pos = m_SelectWindow->GetSize() * 0.5f;
	number->SetPos(pos.x, pos.y);
	float sizeX=m_SizeX->GetFloat();
	float sizeY = m_SizeY->GetFloat();
	number->SetSize(sizeX, sizeY);
	number->SetSpace(-5.f);
	number->SetTexture(name, TEXT("Text//rankingNumber.png"));
	for (int i = 0; i < 10; ++i)
	{
		number->AddFrameData(Vector2(i * sizeX, 0.f), Vector2((i + 1) * sizeX, sizeY));
	}
}

void CUINumberWindow::NumberChangeCallback()
{
	if (!m_SelectWidget)
	{
		return;
	}
	m_SelectWidget->SetImageTint((unsigned char)m_Slider->GetPosition(0), (unsigned char)m_Slider->GetPosition(1),
		(unsigned char)m_Slider->GetPosition(2), (unsigned char)m_Slider->GetPosition(3));
	m_SelectWidget->SetOpacity(m_Slider->GetPosition(3) / 255.f);
	m_SelectWidget->SetPlayTime(m_PlayTime->GetFloat());
	m_SelectWidget->SetPlayScale(m_PlayScale->GetFloat());
	m_SelectWidget->SetNumber(m_NumberInput->GetInt());
	m_SelectWidget->SetSpace(m_SpaceInput->GetFloat());
	m_SelectWidget->SetPlayScale(m_PlayScale->GetFloat());
	m_SelectWidget->SetPos(m_PosX->GetFloat(), m_PosY->GetFloat());
	m_SelectWidget->SetSize(m_SizeX->GetFloat(), m_SizeY->GetFloat());
}

void CUINumberWindow::TextureChangeCallback()
{
	if (!m_SelectWidget)
	{
		return;
	}
	OPENFILENAME	ofn = {};
	TCHAR	fullPath[MAX_PATH] = {};
	TCHAR	filter[] = TEXT("모든 파일\0*.*\0PNG\0*.png\0JPG\0*.jpg\0BMP\0*.bmp\0GIF\0*.gif");
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
		m_SelectWidget->SetTextureFullPath(texName, fullPath);
	}
}