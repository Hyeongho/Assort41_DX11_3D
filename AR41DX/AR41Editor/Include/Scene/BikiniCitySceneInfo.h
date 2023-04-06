#pragma once
#include "Scene/SceneInfo.h"

class CBikiniCitySceneInfo : public CSceneInfo
{
public:
    CBikiniCitySceneInfo();
    ~CBikiniCitySceneInfo();

private:
    class CFade* m_Fade;

public:
    virtual void Start();
    virtual bool Init();
};

