#include "UITextWindow.h"
#include "ResourceWindow.h"
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
#include "Editor/EditorListBox.h"
#include "UI/UIText.h"
#include "UI/UIWindow.h"
#include "Resource/ResourceManager.h"

CUITextWindow::CUITextWindow()
	: m_TransparencyCheck(nullptr)
	, m_AlignHCB(nullptr)
	, m_AlignVCB(nullptr)
	, m_AlignHIndex(0)
	, m_AlignVIndex(0)
	, m_Count(0)
	, m_Slider(nullptr)
	, m_FontList(nullptr)
	, m_TextInput(nullptr)
	, m_FontSizeInput(nullptr)
	, m_PosX(nullptr)
	, m_PosY(nullptr)
	, m_SizeX(nullptr)
	, m_SizeY(nullptr)
	, m_ShadowSlider(nullptr)
	, m_ShadowOffsetX(nullptr)
	, m_ShadowOffsetY(nullptr)
	, m_ShadowEnableCheck(nullptr)
	, m_ShadowTransparencyCheck(nullptr)
{
}

CUITextWindow::~CUITextWindow()
{
}

void CUITextWindow::SetSelectWidget(CUIText* widget)
{
	m_SelectWidget = widget;
	m_TransparencyCheck->SetCheck(m_SelectWidget->GetTransparency());
	m_AlignHIndex = (int)m_SelectWidget->GetAlignH();
	std::string alignHName = m_AlignHCB->GetItem(m_AlignHIndex);
	m_AlignHCB->SetPrevViewName(alignHName);
	m_AlignVIndex = (int)m_SelectWidget->GetAlignV();
	std::string alignVName = m_AlignVCB->GetItem(m_AlignVIndex);
	m_AlignVCB->SetPrevViewName(alignVName);
	m_Slider->SetPosition(0, m_SelectWidget->GetColor().x * 255.f);
	m_Slider->SetPosition(1, m_SelectWidget->GetColor().y * 255.f);
	m_Slider->SetPosition(2, m_SelectWidget->GetColor().z * 255.f);
	m_Slider->SetPosition(3, m_SelectWidget->GetOpacity() * 255.f);
	char	textName[256] = {};
	int length = (int)WideCharToMultiByte(CP_ACP, 0, m_SelectWidget->GetText(), -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, m_SelectWidget->GetText(), -1, textName, length, nullptr, nullptr);
	m_TextInput->SetText(textName);
	m_FontSizeInput->SetFloat(m_SelectWidget->GetFontSize());
	m_PosX->SetFloat(m_SelectWidget->GetPos().x);
	m_PosY->SetFloat(m_SelectWidget->GetPos().y);
	m_SizeX->SetFloat(m_SelectWidget->GetSize().x);
	m_SizeY->SetFloat(m_SelectWidget->GetSize().y);
//shadow
	m_ShadowSlider->SetPosition(0, m_SelectWidget->GetShadowColor().x * 255.f);
	m_ShadowSlider->SetPosition(1, m_SelectWidget->GetShadowColor().y * 255.f);
	m_ShadowSlider->SetPosition(2, m_SelectWidget->GetShadowColor().z * 255.f);
	m_ShadowSlider->SetPosition(3, m_SelectWidget->GetShadowOpacity() * 255.f);
	m_ShadowOffsetX->SetFloat(m_SelectWidget->GetShadowOffset().x);
	m_ShadowOffsetY->SetFloat(m_SelectWidget->GetShadowOffset().y);
	m_ShadowEnableCheck->SetCheck(m_SelectWidget->GetShadowEnable());
	m_ShadowTransparencyCheck->SetCheck(m_SelectWidget->GetShadowTransparency());
}

void CUITextWindow::SetSelectWindow(CUIWindow* window)
{
	m_SelectWindow = window;
}

bool CUITextWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("UI 텍스트");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("생성", 50.f, 30.f);
	button->SetClickCallback<CUITextWindow>(this, &CUITextWindow::TextCreateCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("설정", 50.f, 30.f);
	button->SetClickCallback<CUITextWindow>(this, &CUITextWindow::TextChangeCallback);
	label = CreateWidget<CEditorLabel>("텍스트");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_TransparencyCheck = CreateWidget<CEditorCheckBox>("투명도 체크");
	m_TextInput = CreateWidget<CEditorInput>("텍스트", 300.f, 100.f);
	m_TextInput->SetHideName("텍스트");
	m_TextInput->SetInputType(EImGuiInputType::String);
	m_TextInput->SetMultiLine(true);
	m_AlignHCB = CreateWidget<CEditorComboBox>("수평 정렬");
	m_AlignHCB->SetPrevViewName("Left");
	m_AlignHCB->AddItem("Left");
	m_AlignHCB->AddItem("Center");
	m_AlignHCB->AddItem("Right");
	m_AlignHCB->SetSelectPrevViewName(true);
	m_AlignHCB->SetSelectCallback<CUITextWindow>(this, &CUITextWindow::AlignHCBCallback);
	m_AlignVCB = CreateWidget<CEditorComboBox>("수직 정렬");
	m_AlignVCB->SetPrevViewName("Top");
	m_AlignVCB->AddItem("Top");
	m_AlignVCB->AddItem("Middle");
	m_AlignVCB->AddItem("Bottom");
	m_AlignVCB->SetSelectPrevViewName(true);
	m_AlignVCB->SetSelectCallback<CUITextWindow>(this, &CUITextWindow::AlignVCBCallback);
	m_Slider = CreateWidget<CEditorSlider>("Slider");
	m_Slider->SetText("색상");
	m_FontSizeInput = CreateWidget<CEditorInput>("폰트 크기", 80.f, 30.f);
	m_FontSizeInput->SetInputType(EImGuiInputType::Float);
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
	m_FontList = CreateWidget<CEditorListBox>("FontListBox");
	m_FontList->SetHideName("FontListBox");
	m_FontList->SetSize(200.f, 300.f);
	m_FontList->SetPageItemCount(6);
	m_FontList->SetSelectCallback<CUITextWindow>(this, &CUITextWindow::FontListClickCallback);
//shadow
	label = CreateWidget<CEditorLabel>("그림자 옵션");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_ShadowEnableCheck = CreateWidget<CEditorCheckBox>("그림자 활성화");
	m_ShadowTransparencyCheck = CreateWidget<CEditorCheckBox>("그림자 투명도 체크");
	m_ShadowSlider = CreateWidget<CEditorSlider>("Slider");
	m_ShadowSlider->SetText("그림자 색상");
	m_ShadowOffsetX = CreateWidget<CEditorInput>("X Offset", 80.f, 30.f);
	m_ShadowOffsetX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ShadowOffsetY = CreateWidget<CEditorInput>("Y Offset", 80.f, 30.f);
	m_ShadowOffsetY->SetInputType(EImGuiInputType::Float);
	LoadFontName();
	return true;
}

void CUITextWindow::Update(float deltaTime)
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

void CUITextWindow::AlignHCBCallback(int index, const std::string& item)
{
	m_AlignHIndex = index;
}

void CUITextWindow::AlignVCBCallback(int index, const std::string& item)
{
	m_AlignVIndex = index;
}

void CUITextWindow::FontListClickCallback(int index, const std::string& item)
{
	m_FontItem = item;
}

void CUITextWindow::TextCreateCallback()
{
	if (!m_SelectWindow)
	{
		return;
	}
	++m_Count;
	std::string name = "Text_" + std::to_string(m_Count);
	CUIText* text = m_SelectWindow->CreateWidget<CUIText>(name);
	Vector2 pos = m_SelectWindow->GetSize() * 0.5f;
	text->SetPos(pos.x, pos.y);
	text->SetSize(100.f, 100.f);
	text->SetText(TEXT("Text"));
	text->SetFontSize(30.f);
}

void CUITextWindow::TextChangeCallback()
{
	if (!m_SelectWidget)
	{
		return;
	}
	m_SelectWidget->SetColor((unsigned char)m_Slider->GetPosition(0), (unsigned char)m_Slider->GetPosition(1),
		(unsigned char)m_Slider->GetPosition(2));
	m_SelectWidget->SetOpacity(m_Slider->GetPosition(3) / 255.f);
	m_SelectWidget->SetTransparency(m_TransparencyCheck->GetCheck());
	m_SelectWidget->SetPos(m_PosX->GetFloat(), m_PosY->GetFloat());
	m_SelectWidget->SetSize(m_SizeX->GetFloat(), m_SizeY->GetFloat());
	m_SelectWidget->SetAlignH((Text_Align_H)m_AlignHIndex);
	m_SelectWidget->SetAlignV((Text_Align_V)m_AlignVIndex);

	if(m_FontItem!="")
	{
		m_SelectWidget->SetFont(m_FontItem);
	}
	m_SelectWidget->SetText(m_TextInput->GetText());
	m_SelectWidget->SetFontSize(m_FontSizeInput->GetFloat());
//shadow
	m_SelectWidget->SetShadowColor((unsigned char)m_ShadowSlider->GetPosition(0), 
		(unsigned char)m_ShadowSlider->GetPosition(1), (unsigned char)m_ShadowSlider->GetPosition(2));
	m_SelectWidget->SetShadowOpacity(m_ShadowSlider->GetPosition(3) / 255.f);
	m_SelectWidget->SetShadowOffset(m_ShadowOffsetX->GetFloat(), m_ShadowOffsetY->GetFloat());
	m_SelectWidget->SetShadowEnable(m_ShadowEnableCheck->GetCheck());
	m_SelectWidget->SetShadowTransparency(m_ShadowTransparencyCheck->GetCheck());
}

void CUITextWindow::LoadFontName()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(FONT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->PathMultibyte);
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	const char* textureExtArray = { ".OTF" };
	for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
	{
		char	name[256] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		char	folderName[10] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		int	length = (int)strlen(fullPath);
		if (length >= 6)
		{
			for (int i = length - 7; i >= 0; --i)
			{
				memcpy(folderName, &fullPath[i], 6);
				if (strcmp(folderName, "/Font/") == 0)
				{
					strcpy_s(name, &fullPath[i + 6]);
					break;
				}
			}
		}
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
		_strupr_s(ext);
		bool	fileExt = false;
		if (strcmp(ext, textureExtArray) == 0)
		{
			fileExt = true;
		}
		if (!fileExt)
		{
			continue;
		}
		m_FontList->AddItem(name);
		TCHAR	fileName[MAX_PATH] = {};
		length = (int)MultiByteToWideChar(CP_ACP, 0, name, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, name, -1, fileName, length);
		CResourceManager::GetInst()->CreateFontCollection(name, fileName);
		CResourceManager::GetInst()->LoadFont(name, fileName, 600, 20.f, TEXT("ko"));
	}
}
