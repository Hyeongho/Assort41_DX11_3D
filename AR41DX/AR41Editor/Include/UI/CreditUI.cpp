#include "CreditUI.h"

#include "Device.h"
#include "Engine.h"
#include "Input.h"
#include "PathManager.h"

#include <iostream>
#include <fstream>

CCreditUI::CCreditUI()
{
	m_WindowTypeName = "CreditUI";

	m_Start = false;
	m_ScrollSpeed = 50.f;
}

CCreditUI::CCreditUI(const CCreditUI& Window)
	: CUIWindow(Window)
{
	m_Start = Window.m_Start;
	m_ScrollSpeed = Window.m_ScrollSpeed;
}

CCreditUI::~CCreditUI()
{
}

void CCreditUI::Start()
{
	CUIWindow::Start();

#ifdef _DEBUG
	CInput::GetInst()->AddBindFunction<CCreditUI>("F1", Input_Type::Up, this, &CCreditUI::ActiveAllUI, m_Scene);
	CInput::GetInst()->AddBindFunction<CCreditUI>("F2", Input_Type::Up, this, &CCreditUI::InActiveAllUI, m_Scene);
	CInput::GetInst()->AddBindFunction<CCreditUI>("F3", Input_Type::Up, this, &CCreditUI::CreditPlay, m_Scene);
	CInput::GetInst()->AddBindFunction<CCreditUI>("F4", Input_Type::Up, this, &CCreditUI::CreditStop, m_Scene);
#endif // DEBUG

	CInput::GetInst()->AddBindFunction<CCreditUI>("Space", Input_Type::Push, this, &CCreditUI::SpacePush, m_Scene);
	CInput::GetInst()->AddBindFunction<CCreditUI>("Space", Input_Type::Up, this, &CCreditUI::SpaceUp, m_Scene);
}

bool CCreditUI::Init()
{
	CUIWindow::Init();

	CreateAllUI();

	InActiveAllUI();

	return true;
}

void CCreditUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	if (m_Start) {
		Vector2 Pos = m_TextTitleUI->GetPos();
		Pos.y = Pos.y + g_DeltaTime * m_ScrollSpeed;

		m_TextTitleUI->SetPos(Pos);

		Pos = m_TextUI->GetPos();
		Pos.y = Pos.y + g_DeltaTime * m_ScrollSpeed;

		m_TextUI->SetPos(Pos);

		if (Pos.y >= 0.f) {
			// 타이틀 씬으로 돌아가기

		}

	}
}

void CCreditUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CCreditUI::Render()
{
	CUIWindow::Render();
}

CCreditUI* CCreditUI::Clone()
{
	return new CCreditUI(*this);
}

void CCreditUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CCreditUI::Load(FILE* File)
{
	CUIWindow::Load(File);
}

void CCreditUI::CreditPlay()
{
	m_Start = true;

	ActiveAllUI();
}

void CCreditUI::CreditStop()
{
	m_Start = false;

	InActiveAllUI();


	m_TextTitleUI->SetPos(0.f, 0.f);
	Resolution	RS = CDevice::GetInst()->GetResolution();
	m_TextUI->SetPos(0.f, -(RS.Height * 13.6f));
}

void CCreditUI::SpacePush()
{
	m_ScrollSpeed = 1000.f;
}

void CCreditUI::SpaceUp()
{
	m_ScrollSpeed = 50.f;
}

void CCreditUI::ActiveAllUI()
{
	m_BackUI->SetEnable(true);
	m_TextTitleUI->SetEnable(true);
	m_TextUI->SetEnable(true);
}

void CCreditUI::InActiveAllUI()
{
	m_BackUI->SetEnable(false);
	m_TextTitleUI->SetEnable(false);
	m_TextUI->SetEnable(false);
}

void CCreditUI::CreateAllUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	m_BackUI = CreateWidget<CUIImage>("CreditUI_BackImage");
	m_BackUI->SetTexture("Credit_BackImage", TEXT("UI/CreditBack.png"));
	m_BackUI->SetPos(0.f, 0.f);
	m_BackUI->SetSize(RS.Width, RS.Height);
	m_BackUI->SetOpacity(0.8f);

	m_TextTitleUI = CreateWidget<CUIText>("CreditUI_TextTitle");
	m_TextTitleUI->SetPos(0.f, 0.f);
	m_TextTitleUI->SetText("SPONGEBOB");
	m_TextTitleUI->SetFontSize(80.f);
	m_TextTitleUI->SetSize(RS.Width, RS.Height);
	m_TextTitleUI->SetAlignH(Text_Align_H::Center);
	m_TextTitleUI->SetAlignV(Text_Align_V::Middle);


	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	char	Path[MAX_PATH] = {};
	strcpy_s(Path, Info->PathMultibyte);
	strcat_s(Path, "Dialog/Credit.txt");

	std::string Raw;
	std::string line;
	std::ifstream file(Path);

	if (file.is_open()) {
		while (getline(file, line)) {
			Raw = Raw + line + "\n";
		}
		file.close();
	}

	int rq_cch = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, Raw.c_str(), static_cast<int>(strlen(Raw.c_str())), nullptr, 0);

	std::wstring wValue;
	wValue.clear();
	wValue.resize(rq_cch);

	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, Raw.c_str(),
		static_cast<int>(strlen(Raw.c_str())),
		const_cast<wchar_t*>(wValue.c_str()),
		static_cast<int>(wValue.size()));

	m_TextUI = CreateWidget<CUIText>("CreditUI_Text");
	m_TextUI->SetPos(0.f, -(RS.Height * 13.6f));
	m_TextUI->SetText(wValue.c_str());
	m_TextUI->SetFontSize(40.f);
	m_TextUI->SetSize(RS.Width, RS.Height * 14.f);
	m_TextUI->SetAlignH(Text_Align_H::Center);
	m_TextUI->SetAlignV(Text_Align_V::Top);
}
