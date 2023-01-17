#pragma once
#include "GraphicShader.h"
class CUIShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CUIShader();
    virtual ~CUIShader();

public:
    virtual bool Init();
};

