#pragma once

#include "Scene\SceneInfo.h"

class CTestScene :
    public CSceneInfo
{
public:
    CTestScene();
    ~CTestScene();

public:
    virtual bool Init();
};

