#pragma once

#include "GraphicShader.h"

class CShadowMapStaticInstancingShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CShadowMapStaticInstancingShader();
    virtual ~CShadowMapStaticInstancingShader();

public:
    virtual bool Init();
};

