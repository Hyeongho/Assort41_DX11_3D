#pragma once
#include "GraphicShader.h"
class CDeferredRenderShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CDeferredRenderShader();
    virtual ~CDeferredRenderShader();

public:
    virtual bool Init();
};

