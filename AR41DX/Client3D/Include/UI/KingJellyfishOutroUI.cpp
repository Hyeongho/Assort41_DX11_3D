#include "KingJellyfishOutroUI.h"

#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"

CKingJellyfishOutroUI::CKingJellyfishOutroUI()
{
    m_WindowTypeName = "KingJellyfishOutroUI";
}

CKingJellyfishOutroUI::CKingJellyfishOutroUI(const CKingJellyfishOutroUI& Window) :
    CUIWindow(Window)
{
    this->pGraphBuilder = NULL;
    this->pMediaControl = NULL;
    this->pMediaSeeking = NULL;
}

CKingJellyfishOutroUI::~CKingJellyfishOutroUI()
{
}

bool CKingJellyfishOutroUI::StartDirectShow(const TCHAR* fullPath)
{
    HRESULT hr = 0;

    if (pGraphBuilder)
        return false;

    // Initialize the COM library.
    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        StopDirectShow();
        MessageBox(0, TEXT("ERROR !"), TEXT("ERROR - Could not initialize COM library"), 0);
        return false;
    }

    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&pGraphBuilder);
    if (FAILED(hr)) {
        StopDirectShow();
        MessageBox(0, TEXT("ERROR !"), TEXT("ERROR - Could not create the Filter Graph Manager."), 0);
        return false;
    }

    hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&pMediaControl);
    hr = pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&pMediaEventEx);

    // Build the graph. IMPORTANT: Change this string to a file on your system.
    hr = pGraphBuilder->RenderFile(fullPath, NULL);
    //ThrowIfError(hr);
    if (FAILED(hr)) {
        MessageBox(0, TEXT("ERROR - Could not Find the request file to play."), TEXT("ERROR !"), 0);
        return false;
    }


    if (SUCCEEDED(hr)) {   // Run the graph.
        hr = pMediaControl->Run();

        long evCode = 0;
        pMediaEventEx->WaitForCompletion(INFINITE, &evCode);
    }

    StopDirectShow();
    return true;

}

void CKingJellyfishOutroUI::StopDirectShow()
{
    if (pGraphBuilder) {
        pGraphBuilder->Release();
        pGraphBuilder = NULL;
    }

    if (pMediaControl) {
        pMediaControl->Release();
        pMediaControl = NULL;
    }

    if (pMediaEventEx) {
        pMediaEventEx->Release();
        pMediaEventEx = NULL;
    }

    CoUninitialize();

}

int CKingJellyfishOutroUI::Stop()
{

    if (pGraphBuilder)
    {
        pGraphBuilder->Release();
        pGraphBuilder = NULL;
    }

    if (pMediaControl)
    {
        pMediaControl->Release();
        pMediaControl = NULL;
    }

    if (pMediaEventEx)
    {
        pMediaEventEx->Release();
        pMediaEventEx = NULL;
    }

    CoUninitialize();

    return 0;

}

//void CKingJellyfishOutroUI::LoadComplete()
//{
//    CUIWindow::LoadComplete();
//}

void CKingJellyfishOutroUI::Start()
{
    CUIWindow::Start();
}

bool CKingJellyfishOutroUI::Init()
{
    CUIWindow::Init();

    //StartDirectShow(L("C:\0321\Assort41_DX11_3D\AR41DX\Client3D\Bin\Texture\demo.MP4"));

    HRESULT hr;
    hWnd = CEngine::GetInst()->GetWindowHandle();

    if (pGraphBuilder)
        return false;

    // Initialize the COM library.
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        Stop();
        MessageBox(0, TEXT("ERROR !"), TEXT("ERROR - Could not initialize COM library"), 0);
        return false;
    }

    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&pGraphBuilder);
    if (FAILED(hr))
    {
        Stop();
        MessageBox(0, TEXT("ERROR !"), TEXT("ERROR - Could not create the Filter Graph Manager."), 0);
        return false;
    }

    hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&pMediaControl);
    hr = pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&pMediaEventEx);

    // Build the graph. IMPORTANT: Change this string to a file on your system.
    hr = pGraphBuilder->RenderFile(L"C:\\0329\\Assort41_DX11_3D\\AR41DX\\AR41Editor\\Bin\\Scene\\Cutscene\\2.avi", NULL);
    //ThrowIfError(hr);
    if (FAILED(hr))
        MessageBox(0, TEXT("ERROR - Could not Find the request file to play."), TEXT("ERROR !"), 0);


    if (SUCCEEDED(hr))
    {   // Run the graph.
        hr = pMediaControl->Run();

        long evCode = 0;
        pMediaEventEx->WaitForCompletion(INFINITE, &evCode);
    }

    Stop();

    //CSceneManager::GetInst()->CreateNextScene(true);
    //CSceneManager::GetInst()->CreateSceneInfo<CMainSceneInfo>(false);

    return 0;

}

void CKingJellyfishOutroUI::Update(float DeltaTime)
{
    CUIWindow::Update(DeltaTime);
}

void CKingJellyfishOutroUI::PostUpdate(float DeltaTime)
{
    CUIWindow::Update(DeltaTime);
}

void CKingJellyfishOutroUI::Render()
{
    CUIWindow::Render();
}

CKingJellyfishOutroUI* CKingJellyfishOutroUI::Clone()
{
    return new CKingJellyfishOutroUI(*this);
}

void CKingJellyfishOutroUI::Save(FILE* File)
{
    CUIWindow::Save(File);
}

void CKingJellyfishOutroUI::Load(FILE* File)
{
    CUIWindow::Load(File);
}
