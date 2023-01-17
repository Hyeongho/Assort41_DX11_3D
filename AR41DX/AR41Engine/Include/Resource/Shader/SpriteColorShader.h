#pragma once
#include "GraphicShader.h"
class CSpriteColorShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CSpriteColorShader();
    virtual ~CSpriteColorShader();

public:
    virtual bool Init();
};

