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
#include "InteractUI.h"

//#include "../GameObject/Player.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "../GameObject/Npc/Patric.h"

CDialogUI::CDialogUI() :
	m_curNpc(ENpcList::End)
{
	m_WindowTypeName = "DialogUI";
}

CDialogUI::CDialogUI(const CDialogUI& Window) :
	CUIWindow(Window)
{
	m_curNpc = Window.m_curNpc;
}

CDialogUI::~CDialogUI()
{
	CInteractUI* InteractUI = nullptr;
	if (m_Scene) 
	{
		InteractUI= m_Scene->GetViewport()->FindUIWindow<CInteractUI>("InteractUI");
	}
	if (InteractUI)
		InteractUI->ActiveInteractUI();
}

void CDialogUI::Start()
{
	CUIWindow::Start();

	CInput::GetInst()->AddBindFunction<CDialogUI>("LClick", Input_Type::Up, this, &CDialogUI::KeyLeftButton, m_Scene);
	CInput::GetInst()->AddBindFunction<CDialogUI>("RClick", Input_Type::Up, this, &CDialogUI::KeyRightButton, m_Scene);

	CreaeteAllUI();
}

bool CDialogUI::Init()
{
	CUIWindow::Init();

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
	m_vecWidget.clear();
	CUIWindow::Save(File);
}

void CDialogUI::Load(FILE* File)
{
	CUIWindow::Load(File);

	CreaeteAllUI();
}

void CDialogUI::CreaeteAllUI()
{
	if (!m_mapDialogUI.empty())
		return;

	CreateDialogUI();
	InActiveDialogUI();
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
	Text->SetText("");
	Text->SetFontSize(30.f);
	Text->SetSize(DialogXSize * 0.2f, DialogYSize);
	Text->SetPos(DialogXPos + DialogXSize * 0.05f, 10.f);

	m_mapDialogUI.insert(std::make_pair("DialogUI_TextTalker", Text));


	Text = CreateWidget<CUIText>("DialogUI_TextBox");

	Text->SetAlignH(Text_Align_H::Center);
	Text->SetAlignV(Text_Align_V::Middle);
	Text->SetColor(Vector4::Black);
	Text->SetShadowEnable(false);
	Text->SetText("");
	Text->SetFontSize(30.f);
	Text->SetSize(DialogXSize * 0.6f, DialogYSize);
	Text->SetPos(DialogXPos + DialogXSize * 0.25f, 10.f);

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

bool CDialogUI::GetIsActive()
{
	for (auto iter : m_mapDialogUI)
	{
		return iter.second->GetEnable();
	}
	return false;
}

void CDialogUI::OpenDialog()
{
	KeyLeftButton();
	ActiveDialogUI();

	m_Scene->GetViewport()->FindUIWindow<CInteractUI>("InteractUI")->InActiveInteractUI();
}

void CDialogUI::SetDialogInfo(EMapList Map, ENpcList Npc)
{
	std::string FileName = "";

	switch (Map)
	{
	case EMapList::Bikini_Bottom:
		FileName = "BB";
		break;
	case EMapList::Chum_Bucketlab:
		FileName = "CB";
		break;
	case EMapList::Jelly_Fish_Field:
		FileName = "JFF";
		break;
	}

	FileName += "_";

	m_curNpc = Npc;

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
	case ENpcList::BusDriver:
		FileName += "BusDriver";
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
	std::wstring strTalker = m_curDialog.vecTalker[m_curDialog.TextIdx];
	std::wstring strText = m_curDialog.vecText[m_curDialog.TextIdx];

	CUIText* Text = (CUIText*)m_mapDialogUI.find("DialogUI_TextTalker")->second.Get();
	Text->SetText(strTalker.c_str());

	Text = (CUIText*)m_mapDialogUI.find("DialogUI_TextBox")->second.Get();
	Text->SetText(strText.c_str());


	// Anim play
	std::wstring wstrAnimName = m_curDialog.vecAnim[m_curDialog.TextIdx];
	std::string strAnimName;
	strAnimName.assign(wstrAnimName.begin(), wstrAnimName.end());

	if (wcscmp(strTalker.c_str(), L"스폰지밥") == 0) {

	}
	else {
		CNpc* NpcObjec = nullptr;

		switch (m_curNpc)
		{
		case ENpcList::MrKrabs:
			NpcObjec = (CNpc*)m_Scene->FindObject("MrKrabs");
			break;
		case ENpcList::Squidward:
			NpcObjec = (CNpc*)m_Scene->FindObject("Squidward");
			break;
		case ENpcList::Patric:
			NpcObjec = (CNpc*)m_Scene->FindObject("Patric");
			break;
		case ENpcList::BusDriver:
			NpcObjec = (CNpc*)m_Scene->FindObject("BusDriver");
			break;
		case ENpcList::End:
		default:
			break;
		}

		if (NpcObjec)
			return;

		NpcObjec->ChangeAnimByName(strAnimName);
	}
}

void CDialogUI::KeyRightButton()
{
	return;
}
