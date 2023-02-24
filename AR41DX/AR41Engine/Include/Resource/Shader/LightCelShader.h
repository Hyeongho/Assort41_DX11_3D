#pragma once

#include "GraphicShader.h"

class CLightCelShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CLightCelShader();
    virtual ~CLightCelShader();

public:
    virtual bool Init();
};

