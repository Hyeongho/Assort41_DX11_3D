#include "LoadingUI.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CLoadingUI::CLoadingUI()
{
    m_WindowTypeName = "LoadingUI";
}

CLoadingUI::CLoadingUI(const CLoadingUI& Window) :
    CUIWindow(Window)
{
    m_LoadingBG = FindWidget<CUIImage>("LoadingBG");
    m_LoadingCircle = FindWidget<CUIImage>("LoadingCircle");
    m_LoadingText = FindWidget<CUIText>("Title");
}

CLoadingUI::~CLoadingUI()
{
}

void CLoadingUI::SetLoadingPercent(float Percent)
{
    //m_LoadingBar->SetValue(Percent * 100.f);
}

void CLoadingUI::Start()
{
    CUIWindow::Start();
}

bool CLoadingUI::Init()
{
    CUIWindow::Init();

    m_LoadingBG = CreateWidget<CUIImage>("LoadingBG");
    m_LoadingBG->SetSize(1280.f, 720.f);
    m_LoadingBG->SetTexture("LoadingBG", TEXT("UI\\load_bg.tga"));

    m_LoadingCircle = CreateWidget<CUIImage>("LoadingCircle");
    m_LoadingCircle->SetPos(1250.f, 200.f);
    m_LoadingCircle->SetSize(205.f, 205.f);
    m_LoadingCircle->SetPivot(0.5f, 0.5f);
    m_LoadingCircle->SetTexture("LoadingCircle", TEXT("UI\\loadingcircle.png"));

    m_LoadingText = CreateWidget<CUIText>("loadingText");
    m_LoadingText->SetPos(1110.f, 0.f);
    m_LoadingText->SetSize(300.f, 100.f);
    m_LoadingText->SetPivot(0.5f, 0.5f);
    m_LoadingText->SetFontSize(35.f);
    m_LoadingText->SetText(TEXT("Please wait..."));
    m_LoadingText->SetAlignH(Text_Align_H::Center);
    return true;
}

void CLoadingUI::Update(float DeltaTime)
{
    CUIWindow::Update(DeltaTime);
    m_LoadingCircle->AddAngle(-360.f * DeltaTime);
}

void CLoadingUI::PostUpdate(float DeltaTime)
{
    CUIWindow::PostUpdate(DeltaTime);
}

void CLoadingUI::Render()
{
    CUIWindow::Render();
}

CLoadingUI* CLoadingUI::Clone()
{
    return new CLoadingUI(*this);
}

void CLoadingUI::Save(FILE* File)
{
    CUIWindow::Save(File);
}

void CLoadingUI::Load(FILE* File)
{
    CUIWindow::Load(File);
    m_LoadingBG = FindWidget<CUIImage>("LoadingBG");
    m_LoadingCircle = FindWidget<CUIImage>("LoadingCircle");
    m_LoadingText = FindWidget<CUIText>("Title");
}
