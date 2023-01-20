#include "LoadingUI.h"
#include "UI/UIProgressBar.h"
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
}

CLoadingUI::~CLoadingUI()
{
}

void CLoadingUI::SetLoadingPercent(float Percent)
{
    m_LoadingBar->SetValue(Percent * 100.f);
}

void CLoadingUI::Start()
{
    CUIWindow::Start();
}

bool CLoadingUI::Init()
{
    CUIWindow::Init();

    m_Back = CreateWidget<CUIImage>("Back");

    m_Back->SetSize(1280.f, 720.f);
    m_Back->SetTexture("LoadingBack", TEXT("LoadingBack.jpg"));

    m_LoadingBar = CreateWidget<CUIProgressBar>("LoadingBar");

    m_LoadingBar->SetPos(100.f, 50.f);
    m_LoadingBar->SetSize(1080.f, 40.f);

    m_LoadingBar->SetImageTint(EProgressBarTextureType::Back, 100, 100, 100, 255);
    m_LoadingBar->SetImageTint(EProgressBarTextureType::Bar, 255, 255, 255, 255);

    m_LoadingBar->SetTexture(EProgressBarTextureType::Bar, "HPBar",
        TEXT("HPBar.png"));

    m_LoadingBar->SetProgressBarMin(0.f);
    m_LoadingBar->SetProgressBarMax(100.f);
    m_LoadingBar->SetValue(0.f);
    m_LoadingBar->SetBarDir(EProgressBarDir::RightToLeft);

    return true;
}

void CLoadingUI::Update(float DeltaTime)
{
    CUIWindow::Update(DeltaTime);
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
}
