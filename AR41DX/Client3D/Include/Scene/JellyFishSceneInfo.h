#pragma once

#include "Scene/SceneInfo.h"

class CJellyFishSceneInfo :
    public CSceneInfo
{
public:
    CJellyFishSceneInfo();
    ~CJellyFishSceneInfo();

public:
    virtual bool Init();
};

