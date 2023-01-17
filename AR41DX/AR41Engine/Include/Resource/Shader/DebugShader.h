#pragma once
#include "GraphicShader.h"
class CDebugShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CDebugShader();
    virtual ~CDebugShader();

public:
    virtual bool Init();
};

