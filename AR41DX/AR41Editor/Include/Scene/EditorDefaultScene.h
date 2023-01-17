#pragma once

#include "Scene/SceneInfo.h"

class CEditorDefaultScene :
    public CSceneInfo
{
public:
    CEditorDefaultScene();
    ~CEditorDefaultScene();

public:
    virtual bool Init();
};

