
#pragma once

#include "GraphicShader.h"

class CFXAAShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CFXAAShader();
    virtual ~CFXAAShader();

public:
    virtual bool Init();
};
