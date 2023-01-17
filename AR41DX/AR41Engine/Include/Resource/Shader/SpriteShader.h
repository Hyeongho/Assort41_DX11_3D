#pragma once
#include "GraphicShader.h"
class CSpriteShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CSpriteShader();
    virtual ~CSpriteShader();

public:
    virtual bool Init();
};

