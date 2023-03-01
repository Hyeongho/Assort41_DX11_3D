#include "PlayerUI.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Device.h"
#include "Engine.h"
#include "Input.h"

CPlayerUI::CPlayerUI() :
	m_Timer(0.f),
	m_Tabbed(false),
	m_Boss(false),
  m_AllOpacity(3.f)
{
	m_WindowTypeName = "PlayerUI";
}

CPlayerUI::CPlayerUI(const CPlayerUI& Window)
	: CUIWindow(Window)
	, m_AllOpacity(3.f)
{
	m_Timer = 0.f;
	m_Tabbed = false;
	m_Boss = false;

	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		m_MaxHP[i] = FindWidget<CUIImage>("MaxHP" + std::to_string(i));
		m_CurHP[i] = FindWidget<CUIImage>("CurHP" + std::to_string(i));
	}
	m_Socks = FindWidget<CUIImage>("Socks");
	m_Fritter = FindWidget<CUIImage>("Fritter");
	m_Glittering = FindWidget<CUIImage>("Glittering");

	m_SocksText = FindWidget<CUIText>("SocksText");
	m_FritterText = FindWidget<CUIText>("FritterText");
	m_GlitterText = FindWidget<CUIText>("GlitterText");
}

CPlayerUI::~CPlayerUI()
{
}

void CPlayerUI::Start()
{
	CUIWindow::Start();

	CInput::GetInst()->AddBindFunction<CPlayerUI>("Tab", Input_Type::Up, this, &CPlayerUI::KeyTab, m_Scene);
}

bool CPlayerUI::Init()
{
	CUIWindow::Init();

	CreateHpUI();
	CreateHudUI();

	InActiveHpUI();
	InActiveHudUI();

	//m_MaxHP = CreateWidget<CUIImage>("MaxHP");
	//m_MaxHP->SetSize(70.f, 70.f);
	//m_MaxHP->SetPos(20.f, 630.f);
	//m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	//m_MaxHP = CreateWidget<CUIImage>("MaxHP2");
	//m_MaxHP->SetSize(70.f, 70.f);
	//m_MaxHP->SetPos(95.f, 630.f);
	//m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	//m_MaxHP = CreateWidget<CUIImage>("MaxHP3");
	//m_MaxHP->SetSize(70.f, 70.f);
	//m_MaxHP->SetPos(170.f, 630.f);
	//m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	//m_MaxHP = CreateWidget<CUIImage>("MaxHP4");
	//m_MaxHP->SetSize(70.f, 70.f);
	//m_MaxHP->SetPos(245.f, 630.f);
	//m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	//m_MaxHP = CreateWidget<CUIImage>("MaxHP5");
	//m_MaxHP->SetSize(70.f, 70.f);
	//m_MaxHP->SetPos(320.f, 630.f);
	//m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	//m_CurHP = CreateWidget<CUIImage>("CurHP");
	//m_CurHP->SetSize(50.f, 50.f);
	//m_CurHP->SetPos(30.f, 635.f);
	//m_CurHP->SetTexture("CurHP", TEXT("UI/Underpants.png"));

	//m_Fritter = CreateWidget<CUIImage>("Fritter");
	//m_Fritter->SetSize(120.f, 120.f);
	//m_Fritter->SetPos(600.f, 600.f);
	//m_Fritter->SetTexture("Fritter", TEXT("UI/Spatula.png"));

	//m_Glittering = CreateWidget<CUIImage>("Glittering");
	//m_Glittering->SetSize(70.f, 70.f);
	//m_Glittering->SetPos(1180.f, 630.f);
	//m_Glittering->SetTexture("Glittering", TEXT("UI/Flower.png"));

	//m_Socks = CreateWidget<CUIImage>("Socks");
	//m_Socks->SetSize(100.f, 85.f);
	//m_Socks->SetPos(1140.f, 30.f);
	//m_Socks->SetTexture("Socks", TEXT("UI/Sock.png"));

	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		m_MaxHP[i] = CreateWidget<CUIImage>("MaxHP" + std::to_string(i));
		m_MaxHP[i]->SetSize(70.f, 70.f);
		m_MaxHP[i]->SetPos(20.f+75.f*i, 630.f);
		m_MaxHP[i]->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

		m_CurHP[i] = CreateWidget<CUIImage>("CurHP" + std::to_string(i));
		m_CurHP[i]->SetSize(50.f, 50.f);
		m_CurHP[i]->SetPos(30.f + 75.f * i, 635.f);
		m_CurHP[i]->SetTexture("CurHP", TEXT("UI/Underpants.png"));
	}

	m_Fritter = CreateWidget<CUIImage>("Fritter");
	m_Fritter->SetSize(120.f, 120.f);
	m_Fritter->SetPos(600.f, 600.f);
	m_Fritter->SetTexture("Fritter", TEXT("UI/Spatula.png"));

	m_Glittering = CreateWidget<CUIImage>("Glittering");
	m_Glittering->SetSize(70.f, 70.f);
	m_Glittering->SetPos(1180.f, 630.f);
	m_Glittering->SetTexture("Glittering", TEXT("UI/Flower.png"));

	m_Socks = CreateWidget<CUIImage>("Socks");
	m_Socks->SetSize(100.f, 85.f);
	m_Socks->SetPos(1140.f, 30.f);
	m_Socks->SetTexture("Socks", TEXT("UI/Sock.png"));

	m_SocksText = CreateWidget<CUIText>("SocksText");
	m_SocksText->SetPos(1060.f, 50.f);
	m_SocksText->SetSize(60.f, 50.f);
	m_SocksText->SetPivot(0.5f, 0.5f);
	m_SocksText->SetFontSize(50.f);
	m_SocksText->SetText(TEXT("00"));
	m_SocksText->SetTransparency(true);
	m_SocksText->SetAlignH(Text_Align_H::Right);

	m_FritterText = CreateWidget<CUIText>("FritterText");
	m_FritterText->SetPos(700.f, 640.f);
	m_FritterText->SetSize(60.f, 50.f);
	m_FritterText->SetPivot(0.5f, 0.5f);
	m_FritterText->SetFontSize(50.f);
	m_FritterText->SetText(TEXT("00"));
	m_FritterText->SetTransparency(true);
	m_FritterText->SetAlignH(Text_Align_H::Right);

	m_GlitterText = CreateWidget<CUIText>("GlitterText");
	m_GlitterText->SetPos(970.f, 650.f);
	m_GlitterText->SetSize(200.f, 40.f);
	m_GlitterText->SetPivot(0.5f, 0.5f);
	m_GlitterText->SetFontSize(40.f);
	m_GlitterText->SetText(TEXT("0"));
	m_GlitterText->SetTransparency(true);
	m_GlitterText->SetAlignH(Text_Align_H::Right);
	return true;
}

void CPlayerUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	if (m_Tabbed) {
		m_Timer += DeltaTime;

		if (m_Timer >= 4.f) {
			m_Tabbed = false;

			InActiveHudUI();

			if (!m_Boss)
				InActiveHpUI();
		}

	if(m_AllOpacity>0.f)
	{
		m_AllOpacity -= DeltaTime;
		SetPlayerUIOpacity(m_AllOpacity);
	}
}

void CPlayerUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CPlayerUI::Render()
{
	CUIWindow::Render();
}

CPlayerUI* CPlayerUI::Clone()
{
	return new CPlayerUI(*this);
}

void CPlayerUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CPlayerUI::Load(FILE* File)
{
	CUIWindow::Load(File);
}

void CPlayerUI::CreateHpUI()
{
	const std::string HpStand = "PlayerUI_HP";
	float HpBackSize = 70.f;
	float HpXSize = 60.f;
	float HpySize = HpXSize * 0.65f;
	float HpXPos = 20.f;
	float HpYPos = 630.f;

	for (int i = 0; i < 5; i++) {
		std::string HpUIName = HpStand + "BackBubble" + std::to_string(i + 1);

		CUIImage* Image = CreateWidget<CUIImage>(HpUIName);
		Image->SetTexture("PlayerHpBack", TEXT("UI/Hud/Bubble_01.tga"));
		Image->SetSize(HpBackSize, HpBackSize);
		Image->SetPos(HpXPos, HpYPos);

		m_mapHP.insert(std::make_pair(HpUIName, Image));


		HpUIName = HpStand + "Pants" + std::to_string(i + 1);

		Image = CreateWidget<CUIImage>(HpUIName);
		Image->SetTexture("PlayerHpPants", TEXT("UI/Hud/Underpants_01.tga"));
		Image->SetSize(HpXSize, HpySize);

		float Gap = (HpBackSize - HpySize) / 2.f;

		Image->SetPos(HpXPos + 5.f, HpYPos + Gap);

		m_mapHP.insert(std::make_pair(HpUIName, Image));


		HpXPos += (HpBackSize + 5.f);
	}
}

void CPlayerUI::CreateHudUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();


	float GapStand = 20.f;

	float UISize = 70.f;
	float FritXSize = UISize * 0.83;

	float YRightUpPos = RS.Height - UISize - GapStand;


	

	CUIImage* Image = CreateWidget<CUIImage>("PlayerUI_ImageFritter");
	Image->SetSize(FritXSize, UISize);
	Image->SetPos(RS.Width / 2.f - FritXSize / 2.f, YRightUpPos);
	Image->SetTexture("PlayerSpatula", TEXT("UI/Hud/Spatula_UI.tga"));
	m_mapHud.insert(std::make_pair("PlayerUI_ImageFritter", Image));


	float GlitXSize = UISize * 0.88f;

	Image = CreateWidget<CUIImage>("PlayerUI_ImageGlittering");
	Image->SetSize(GlitXSize, UISize);
	Image->SetPos(RS.Width - GapStand - GlitXSize, YRightUpPos);
	Image->SetTexture("PlayerFlower", TEXT("UI/Hud/Flower_01.tga"));
	m_mapHud.insert(std::make_pair("PlayerUI_ImageGlittering", Image));


	float SocksXSize = UISize * 1.08f;

	Image = CreateWidget<CUIImage>("PlayerUI_ImageSocks");
	Image->SetSize(SocksXSize, UISize);
	Image->SetPos(RS.Width - GapStand - SocksXSize, GapStand);
	Image->SetTexture("PlayerSocks", TEXT("UI/Hud/Sock_01.tga"));
	m_mapHud.insert(std::make_pair("PlayerUI_ImageSocks", Image));



}

void CPlayerUI::ActiveHpUI()
{
	for (auto iter : m_mapHP)
		iter.second->SetEnable(true);
}

void CPlayerUI::ActiveHudUI()
{
	for (auto iter : m_mapHud)
		iter.second->SetEnable(true);
}

void CPlayerUI::InActiveHpUI()
{
	for (auto iter : m_mapHP)
		iter.second->SetEnable(false);
}

void CPlayerUI::InActiveHudUI()
{
	for (auto iter : m_mapHud)
		iter.second->SetEnable(false);
}

void CPlayerUI::KeyTab()
{
	ActiveHudUI();
	ActiveHpUI();


	m_Tabbed = true;
	m_Timer = 0.f;

	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		m_MaxHP[i] = FindWidget<CUIImage>("MaxHP" + std::to_string(i));
		m_CurHP[i] = FindWidget<CUIImage>("CurHP" + std::to_string(i));
	}
	m_Socks = FindWidget<CUIImage>("Socks");
	m_Fritter = FindWidget<CUIImage>("Fritter");
	m_Glittering = FindWidget<CUIImage>("Glittering");
	m_SocksText = FindWidget<CUIText>("SocksText");
	m_FritterText = FindWidget<CUIText>("FritterText");
	m_GlitterText = FindWidget<CUIText>("GlitterText");
}

void CPlayerUI::SetPlayerUIOpacity(float opacity)
{
	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		m_MaxHP[i]->SetOpacity(opacity);
		m_CurHP[i]->SetOpacity(opacity);
	}
	m_Socks->SetOpacity(opacity);
	m_Fritter->SetOpacity(opacity);
	m_Glittering->SetOpacity(opacity);

	m_SocksText->SetOpacity(opacity);
	m_FritterText->SetOpacity(opacity);
	m_GlitterText->SetOpacity(opacity);
}

void CPlayerUI::SetHp(int hp)
{
	size_t size = std::size(m_CurHP);
	for (size_t i = 0; i < size; ++i)
	{
		if (hp > i)
		{
			m_CurHP[i]->SetEnable(true);
		}
		else
		{
			m_CurHP[i]->SetEnable(false);
		}
	}
}

void CPlayerUI::SetMaxHp(int hp)
{
	size_t size = std::size(m_MaxHP);
	for (size_t i = 0; i < size; ++i)
	{
		if (hp > i)
		{
			m_MaxHP[i]->SetEnable(true);
		}
		else
		{
			m_MaxHP[i]->SetEnable(false);
		}
	}
}

void CPlayerUI::SetGlitter(int i)
{
	m_GlitterText->SetIntText(i);
}

void CPlayerUI::SetSocks(int i)
{
	m_SocksText->SetIntText(i,true);
}

void CPlayerUI::SetFritter(int i)
{
	m_FritterText->SetIntText(i, true);
}
