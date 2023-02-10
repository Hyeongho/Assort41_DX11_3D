#include "PlayerUI.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CPlayerUI::CPlayerUI()
{
	m_WindowTypeName = "PlayerUI";
}

CPlayerUI::CPlayerUI(const CPlayerUI& Window)
{
	m_MaxHP = FindWidget<CUIImage>("MaxHP");
	m_CurHP = FindWidget<CUIImage>("CurHP");
	m_Socks = FindWidget<CUIImage>("Socks");
	m_Fritter = FindWidget<CUIImage>("Fritter");
	m_Glittering = FindWidget<CUIImage>("Glittering");
}

CPlayerUI::~CPlayerUI()
{
}

void CPlayerUI::Start()
{
	CUIWindow::Start();
}

bool CPlayerUI::Init()
{
	CUIWindow::Init();

	m_MaxHP = CreateWidget<CUIImage>("MaxHP");
	m_MaxHP->SetSize(70.f, 70.f);
	m_MaxHP->SetPos(20.f, 630.f);
	m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	m_MaxHP = CreateWidget<CUIImage>("MaxHP2");
	m_MaxHP->SetSize(70.f, 70.f);
	m_MaxHP->SetPos(95.f, 630.f);
	m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	m_MaxHP = CreateWidget<CUIImage>("MaxHP3");
	m_MaxHP->SetSize(70.f, 70.f);
	m_MaxHP->SetPos(170.f, 630.f);
	m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	m_MaxHP = CreateWidget<CUIImage>("MaxHP4");
	m_MaxHP->SetSize(70.f, 70.f);
	m_MaxHP->SetPos(245.f, 630.f);
	m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	m_MaxHP = CreateWidget<CUIImage>("MaxHP5");
	m_MaxHP->SetSize(70.f, 70.f);
	m_MaxHP->SetPos(320.f, 630.f);
	m_MaxHP->SetTexture("MaxHP", TEXT("UI/Bubble.png"));

	m_CurHP = CreateWidget<CUIImage>("CurHP");
	m_CurHP->SetSize(50.f, 50.f);
	m_CurHP->SetPos(30.f, 635.f);
	m_CurHP->SetTexture("CurHP", TEXT("UI/Underpants.png"));

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

	return true;
}

void CPlayerUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
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

	m_MaxHP = FindWidget<CUIImage>("MaxHP");
	m_CurHP = FindWidget<CUIImage>("CurHP");
	m_Socks = FindWidget<CUIImage>("Socks");
	m_Fritter = FindWidget<CUIImage>("Fritter");
	m_Glittering = FindWidget<CUIImage>("Glittering");
}
