#pragma once

#include "GraphicShader.h"

class CShadowMapStaticShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CShadowMapStaticShader();
    virtual ~CShadowMapStaticShader();

public:
    virtual bool Init();
};

