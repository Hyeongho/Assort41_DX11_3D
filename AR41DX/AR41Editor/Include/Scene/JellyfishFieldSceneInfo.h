#pragma once

#include "Scene\SceneInfo.h"

class CJellyfishFieldSceneInfo :
    public CSceneInfo
{
public:
    CJellyfishFieldSceneInfo();
    ~CJellyfishFieldSceneInfo();

public:
    virtual bool Init();
};