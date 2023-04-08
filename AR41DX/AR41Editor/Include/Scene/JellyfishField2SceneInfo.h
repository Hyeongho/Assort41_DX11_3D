#pragma once

#include "Scene/SceneInfo.h"

class CJellyfishField2SceneInfo :
    public CSceneInfo
{
public:
    CJellyfishField2SceneInfo();
    ~CJellyfishField2SceneInfo();

private:
    class CFade* m_Fade;

public:
    virtual void Start();
    virtual bool Init();
};

