#pragma once

#include "Scene\SceneInfo.h"

class CTitleSceneInfo :
    public CSceneInfo
{
public:
    CTitleSceneInfo();
    ~CTitleSceneInfo();

public:
    virtual bool Init();
};

