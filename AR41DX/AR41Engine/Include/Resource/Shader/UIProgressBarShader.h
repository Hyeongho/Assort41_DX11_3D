#pragma once

#include "GraphicShader.h"

class CUIProgressBarShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CUIProgressBarShader();
    virtual ~CUIProgressBarShader();

public:
    virtual bool Init();
};

