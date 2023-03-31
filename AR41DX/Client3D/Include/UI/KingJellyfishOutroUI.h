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

    IGraphBuilder* pGraphBuilder = nullptr;   // ���� �׷��� �Ŵ��� ������ �ϴ� �������̽�
    IMediaControl* pMediaControl = nullptr;   // ���/�Ͻ�����/���� ���� �����ϱ� ���� �������̽�
    IMediaSeeking* pMediaSeeking = nullptr;   // ��� ��ġ�� �о���ų� �����ϱ� ���� �������̽�
    IMediaEventEx* pMediaEventEx = nullptr;   // ���� �׷����κ��� �̺�Ʈ�� �ޱ� ���� �������̽�
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

