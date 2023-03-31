#pragma once

#include "UI\UIWindow.h"

#include "DShow.h"

class CKingJellyfishOutroUI :
    public CUIWindow
{
    friend class CSceneViewport;
    friend class CScene;

protected:
    CKingJellyfishOutroUI();
    CKingJellyfishOutroUI(const CKingJellyfishOutroUI& Window);
    virtual ~CKingJellyfishOutroUI();

public:

    HWND   hWnd;
    RECT   WindowRC;
    HRESULT hr;

    IGraphBuilder* pGraphBuilder = nullptr;   // 필터 그래프 매니저 역할을 하는 인터페이스
    IMediaControl* pMediaControl = nullptr;   // 재생/일시정지/정지 등을 수행하기 위한 인터페이스
    IMediaSeeking* pMediaSeeking = nullptr;   // 재생 위치를 읽어오거나 변경하기 위한 인터페이스
    IMediaEventEx* pMediaEventEx = nullptr;   // 필터 그래프로부터 이벤트를 받기 위한 인터페이스
    IVideoWindow* pVidWin = NULL;

    bool StartDirectShow(const TCHAR* fullPath);
    void StopDirectShow();
    int  Stop();

public:
    //  virtual void LoadComplete();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CKingJellyfishOutroUI* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

