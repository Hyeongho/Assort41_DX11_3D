#pragma once

#include "Scene\SceneInfo.h"

class CMainSceneInfo :
    public CSceneInfo
{
public:
    CMainSceneInfo();
    ~CMainSceneInfo();

private:
    class CPlayerUI* m_PlayerUI;
public:
    virtual bool Init();
};

