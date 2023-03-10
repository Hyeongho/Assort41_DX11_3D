#pragma once

#include "Scene\SceneInfo.h"

class CJellyfishFieldSceneInfo :
    public CSceneInfo
{
public:
    CJellyfishFieldSceneInfo();
    ~CJellyfishFieldSceneInfo();

private:
    class CPlayerUI* m_PlayerUI;

public:
    virtual bool Init();
};