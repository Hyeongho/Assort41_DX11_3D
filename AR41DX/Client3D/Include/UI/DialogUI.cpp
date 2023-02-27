#include "DialogUI.h"

#include "Device.h"
#include "Input.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneViewport.h"
#include "Scene/Scene.h"
#include "UI/UIButton.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"
#include "UI/UITextButton.h"
#include "RapidXml/CXmlParser.h"

CDialogUI::CDialogUI()
{
	m_WindowTypeName = "DialogUI";
}

CDialogUI::CDialogUI(const CDialogUI& Window) :
	CUIWindow(Window)
{
}

CDialogUI::~CDialogUI()
{
}

void CDialogUI::Start()
{
	CUIWindow::Start();

	CInput::GetInst()->AddBindFunction<CDialogUI>("LButton", Input_Type::Up, this, &CDialogUI::KeyLeftButton, m_Scene);
	CInput::GetInst()->AddBindFunction<CDialogUI>("RButton", Input_Type::Up, this, &CDialogUI::KeyRightButton, m_Scene);
}

bool CDialogUI::Init()
{
	CUIWindow::Init();

	CreateDialogUI();
	SetDialogInfo();

	return true;
}

void CDialogUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CDialogUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CDialogUI::Render()
{
	CUIWindow::Render();
}

CDialogUI* CDialogUI::Clone()
{
	return new CDialogUI(*this);
}

void CDialogUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CDialogUI::Load(FILE* File)
{
	CUIWindow::Load(File);
}

void CDialogUI::CreateDialogUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	CUIImage* Image = CreateWidget<CUIImage>("DialogUI_Background");
	Image->SetTexture("DialogBackground", TEXT("UI/Dialog/Dialog_Box_Stretched_02.tga"));
	
	float DialogXSize = RS.Width * 0.8f;
	float DialogYSize = DialogXSize * 0.2f;
	Image->SetSize(DialogXSize, DialogYSize);

	float DialogXPos = (RS.Width - DialogXSize) / 2.f;
	Image->SetPos(DialogXPos, 10.f);

	m_mapDialogUI.insert(std::make_pair("DialogUI_Background", Image));


	CUIText* Text = CreateWidget<CUIText>("DialogUI_TextTalker");

	Text->SetAlignH(Text_Align_H::Center);
	Text->SetAlignV(Text_Align_V::Middle);
	Text->SetColor(Vector4::Green);
	Text->SetShadowEnable(false);
	Text->SetText("test : ");
	Text->SetFontSize(30.f);
	Text->SetSize(DialogXSize * 0.2f , DialogYSize);
	Text->SetPos(DialogXPos + DialogXSize * 0.05f, 10.f);

	m_mapDialogUI.insert(std::make_pair("DialogUI_TextTalker", Text));


	Text = CreateWidget<CUIText>("DialogUI_TextBox");

	Text->SetAlignH(Text_Align_H::Center);
	Text->SetAlignV(Text_Align_V::Middle);
	Text->SetColor(Vector4::Black);
	Text->SetShadowEnable(false);
	Text->SetText("Blah BlahBlahBlah BlahBlah BlahBlah");
	Text->SetFontSize(30.f);
	Text->SetSize(DialogXSize * 0.6f, DialogYSize);
	Text->SetPos(DialogXPos + DialogXSize * 0.2f, 10.f);

	m_mapDialogUI.insert(std::make_pair("DialogUI_TextBox", Text));
}

void CDialogUI::ActiveDialogUI()
{
	for (auto iter : m_mapDialogUI)
		iter.second->SetEnable(true);
}

void CDialogUI::InActiveDialogUI()
{
	for (auto iter : m_mapDialogUI)
		iter.second->SetEnable(false);
}

void CDialogUI::SetDialogInfo()
{
	// FileSystem으로 해당하는 텍스트 찾아 구조체에 넣는다.

	// 아래는 테스트용 구조체 생성
	int TextCount = 5;
	m_curDialog.TextMaxIdx = TextCount;
	m_curDialog.TextIdx = 0;

	m_curDialog.vecTalker.resize(TextCount);
	m_curDialog.vecText.resize(TextCount);

	m_curDialog.vecTalker[0] = "Sponge";
	m_curDialog.vecTalker[1] = "Sponge";
	m_curDialog.vecTalker[2] = "Sandy";
	m_curDialog.vecTalker[3] = "Sandy";
	m_curDialog.vecTalker[4] = "Sponge";

	m_curDialog.vecText[0] = "Hello Sandy?";
	m_curDialog.vecText[1] = "Good day. Do u think so too?";
	m_curDialog.vecText[2] = "Oh. Spongebob.";
	m_curDialog.vecText[3] = "I don't think so.";
	m_curDialog.vecText[4] = "Oh... okok.";


	CUIText* Text = (CUIText*)m_mapDialogUI.find("DialogUI_TextTalker")->second.Get();
	std::string strTalker = m_curDialog.vecTalker[0] + ":";
	Text->SetText(strTalker.c_str());


	Text = (CUIText*)m_mapDialogUI.find("DialogUI_TextBox")->second.Get();
	Text->SetText(m_curDialog.vecText[0].c_str());

}

void CDialogUI::SetDialogInfo(EMapList Map, ENpcList Npc)
{
	std::string FileName = "";

	switch (Map)
	{
	case EMapList::Patric:
	case EMapList::Krabs:
	case EMapList::Bikini_Bottom:
		FileName = "BB";
		break;
	case EMapList::DutchMan_Grave:
		FileName = "DMG";
		break;
	case EMapList::Sand_Mountain:
		FileName = "SM";
		break;
	case EMapList::Goo_Lagoon:
		FileName = "GL";
		break;
	case EMapList::Dome:
		FileName = "Dome";
		break;
	case EMapList::Krusty_Krab:
		FileName = "KK";
		break;
	case EMapList::Chum_Bucketlab:
		FileName = "CB";
		break;
	case EMapList::Merma_Lair:
		FileName = "ML";
		break;
	case EMapList::Down_Town:
		FileName = "DT";
		break;
	case EMapList::Kelp_Forest:
		FileName = "KF";
		break;
	case EMapList::Rock_Bottom:
		FileName = "RB";
		break;
	case EMapList::Jelly_Fish_Field:
		FileName = "JFF";
		break;
	case EMapList::Industrial_Park:
		FileName = "IP";
		break;
	}

	FileName += "_";

	switch (Npc)
	{
	case ENpcList::MrKrabs:
		FileName += "MrKrabs";
		break;
	case ENpcList::Squidward:
		FileName += "Squidward";
		break;
	case ENpcList::Patric:
		FileName += "Patric";
		break;
	}

	FileName += ".xml";

	CXmlParser* Parser = new CXmlParser;
	Parser->init(FileName);

	int DialCount = Parser->GetDialogCount();

	if (DialCount > 0) {
		m_mapDialogInfo.clear();


		for (int i = 0; i < DialCount; i++) {
			DialogInfo Info;

			std::string DialogName = Parser->GetDialogInfoByIndex(i, &Info);

			m_mapDialogInfo.insert(std::make_pair(DialogName, Info));
		}

	}

	SAFE_DELETE(Parser)
}

void CDialogUI::SetCurDialog(std::string DialName)
{
	m_curDialog = m_mapDialogInfo.find(DialName)->second;
}

void CDialogUI::KeyLeftButton()
{
	m_curDialog.TextIdx++;

	if (m_curDialog.TextIdx >= m_curDialog.TextMaxIdx) {
		// 다이얼로그 UI 창 종료
		InActiveDialogUI();

		return;
	}

	// Sound play



	// Text chnage
	std::string strTalker = m_curDialog.vecTalker[m_curDialog.TextIdx];
	std::string strText = m_curDialog.vecText[m_curDialog.TextIdx];

	CUIText* Text = (CUIText*)m_mapDialogUI.find("DialogUI_TextTalker")->second.Get();
	strTalker += ":";
	Text->SetText(strTalker.c_str());

	Text = (CUIText*)m_mapDialogUI.find("DialogUI_TextBox")->second.Get();
	Text->SetText(strText.c_str());
}

void CDialogUI::KeyRightButton()
{
	SetDialogInfo();
	ActiveDialogUI();

}
