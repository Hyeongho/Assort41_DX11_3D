#include "InteractUI.h"

#include "UI/UIText.h"
#include "UI/UIImage.h"
#include "Device.h"
#include "Scene/SceneViewport.h"
#include "Scene/Scene.h"
#include "Input.h"

CInteractUI::CInteractUI()
{
	m_WindowTypeName = "InteractUI";
}

CInteractUI::CInteractUI(const CInteractUI& Window)
	: CUIWindow(Window)
{
}

CInteractUI::~CInteractUI()
{
}

void CInteractUI::Start()
{
	CUIWindow::Start();

	CreaeteAllUI();
}

bool CInteractUI::Init()
{
	CUIWindow::Init();

	return true;
}

void CInteractUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CInteractUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CInteractUI::Render()
{
	CUIWindow::Render();
}

CInteractUI* CInteractUI::Clone()
{
	return new CInteractUI(*this);
}

void CInteractUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CInteractUI::Load(FILE* File)
{
	CUIWindow::Load(File);

	CreaeteAllUI();
}

void CInteractUI::CreaeteAllUI()
{
	if (!m_mapInteractUI.empty())
		return;
		
	CreateInteractUI();
	InActiveInteractUI();
}

void CInteractUI::CreateInteractUI()
{

	Resolution	RS = CDevice::GetInst()->GetResolution();

	// 하단 가이드
	float GuideSizeX = 25.f;
	float GuideSizeY = 25.f;
	float GuidePosY = 25.f;


	CUIImage* Image = CreateWidget<CUIImage>("InteractUI_ImageGuide");

	Image->SetTexture("KeyGuideMouseLeft", TEXT("UI/Key/KEY_F_InGame.tga"));

	Image->SetSize(GuideSizeX, GuideSizeY);
	Image->SetPos(RS.Width/2.f - GuideSizeX * 2.5f, GuidePosY);

	m_mapInteractUI.insert(std::make_pair("InteractUI_ImageGuide", Image));



	CUIText* Text = CreateWidget<CUIText>("InteractUI_TextGuide");

	Text->SetSize(GuideSizeX * 15.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Left);
	Text->SetPos(RS.Width / 2.f - GuideSizeX, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("를 입력하여 하기"));
	Text->SetColor(Vector4::White);

	m_mapInteractUI.insert(std::make_pair("InteractUI_TextGuide", Text));
}

void CInteractUI::ActiveInteractUI()
{
	for (auto iter : m_mapInteractUI)
		iter.second->SetEnable(true);
}

void CInteractUI::InActiveInteractUI()
{
	for (auto iter : m_mapInteractUI)
		iter.second->SetEnable(false);
}

void CInteractUI::SetTarget(EInteractTarget Target)
{
	CUIText* Text = (CUIText*) m_mapInteractUI.find("InteractUI_TextGuide")->second.Get();


	switch (Target)
	{
	case EInteractTarget::Npc:
		Text->SetText(TEXT("를 눌러 대화하기"));
		break;
	case EInteractTarget::Sponge_House:
		Text->SetText(TEXT("를 눌러 스폰지밥의 집에 입장하기"));
		break;
	case EInteractTarget::Squid_House:
		Text->SetText(TEXT("를 눌러 징징이의 집에 입장하기"));
		break;
	case EInteractTarget::Patric_House:
		Text->SetText(TEXT("를 눌러 뚱이의 집에 입장하기"));
		break;
	case EInteractTarget::Door:
	case EInteractTarget::Gate:
		Text->SetText(TEXT("를 눌러 입장하기"));
		break;
	case EInteractTarget::Teleport_Box:
		Text->SetText(TEXT("를 눌러 텔레포트 박스 사용하기"));
		break;
	}
}