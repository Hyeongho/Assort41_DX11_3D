#pragma once

#include "Scene/SceneInfo.h"

class CMainSceneInfo :
    public CSceneInfo
{
public:
    CMainSceneInfo();
    ~CMainSceneInfo();

public:
    virtual bool Init();
};

