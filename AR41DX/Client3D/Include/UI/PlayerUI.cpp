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
	m_Boss(false)
{
	m_WindowTypeName = "PlayerUI";
}

CPlayerUI::CPlayerUI(const CPlayerUI& Window) :
	CUIWindow(Window)
{
	m_Timer = 0.f;
	m_Tabbed = false;
	m_Boss = false;
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
}
