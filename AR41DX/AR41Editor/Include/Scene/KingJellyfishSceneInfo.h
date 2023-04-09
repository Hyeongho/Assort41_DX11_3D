#pragma once
#include "Scene\SceneInfo.h"

class CKingJellyfishSceneInfo : public CSceneInfo
{
public:
    CKingJellyfishSceneInfo();
    ~CKingJellyfishSceneInfo();

private:
    class CFade* m_Fade;

public:
    virtual void Start();
    virtual bool Init();
};

