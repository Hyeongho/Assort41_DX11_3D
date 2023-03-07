#pragma once

#include "Scene/SceneInfo.h"

class CTestSceneInfo :
    public CSceneInfo
{
public:
    CTestSceneInfo();
    ~CTestSceneInfo();

private:
    class CPlayerUI* m_PlayerUI;
public:
    virtual bool Init();
};

