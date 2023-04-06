#pragma once

#include "Scene/SceneInfo.h"

class CJellyfishField1SceneInfo :
    public CSceneInfo
{
public:
    CJellyfishField1SceneInfo();
    ~CJellyfishField1SceneInfo();

private:
    class CFade* m_Fade;

public:
    virtual void Start();
    virtual bool Init();
};

