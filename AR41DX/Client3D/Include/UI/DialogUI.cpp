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
    m_DialogInfo.TextMaxIdx = TextCount;
    m_DialogInfo.TextIdx = 0;

    m_DialogInfo.vecTalker.resize(TextCount);
    m_DialogInfo.vecText.resize(TextCount);

    m_DialogInfo.vecTalker[0] = "Sponge";
    m_DialogInfo.vecTalker[1] = "Sponge";
    m_DialogInfo.vecTalker[2] = "Sandy";
    m_DialogInfo.vecTalker[3] = "Sandy";
    m_DialogInfo.vecTalker[4] = "Sponge";

    m_DialogInfo.vecText[0] = "Hello Sandy?";
    m_DialogInfo.vecText[1] = "Good day. Do u think so too?";
    m_DialogInfo.vecText[2] = "Oh. Spongebob.";
    m_DialogInfo.vecText[3] = "I don't think so.";
    m_DialogInfo.vecText[4] = "Oh... okok.";


    CUIText* Text = (CUIText*)m_mapDialogUI.find("DialogUI_TextTalker")->second.Get();
    std::string strTalker = m_DialogInfo.vecTalker[0] + ":";
    Text->SetText(strTalker.c_str());


    Text = (CUIText*)m_mapDialogUI.find("DialogUI_TextBox")->second.Get();
    Text->SetText(m_DialogInfo.vecText[0].c_str());

}

void CDialogUI::KeyLeftButton()
{
    m_DialogInfo.TextIdx++;

    if (m_DialogInfo.TextIdx >= m_DialogInfo.TextMaxIdx) {
        // 다이얼로그 UI 창 종료
        InActiveDialogUI();

        return;
    }

    // Sound play



    // Text chnage
    std::string strTalker = m_DialogInfo.vecTalker[m_DialogInfo.TextIdx];
    std::string strText = m_DialogInfo.vecText[m_DialogInfo.TextIdx];

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
