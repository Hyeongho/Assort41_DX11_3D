#pragma once

#include "Scene\SceneInfo.h"

class CRoboSpongeSceneInfo :
    public CSceneInfo
{
public:
    CRoboSpongeSceneInfo();
    ~CRoboSpongeSceneInfo();

public:
    virtual bool Init();
};

