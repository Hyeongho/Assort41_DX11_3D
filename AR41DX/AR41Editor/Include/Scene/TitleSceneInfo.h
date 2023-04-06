#pragma once

#include "Scene\SceneInfo.h"

class CTitleSceneInfo :
    public CSceneInfo
{
public:
    CTitleSceneInfo();
    ~CTitleSceneInfo();

private:
    class CFade* m_Fade;

public:
    virtual void Start();
    virtual bool Init();
};

