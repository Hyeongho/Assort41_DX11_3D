#pragma once

#include "Scene\SceneInfo.h"

class CRoboSpongeSceneInfo :
    public CSceneInfo
{
public:
    CRoboSpongeSceneInfo();
    ~CRoboSpongeSceneInfo();

private:
    class CFade* m_Fade;

public:
    virtual void Start();
    virtual bool Init();
};

