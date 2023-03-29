#pragma once

#include "GraphicShader.h"

class CCartoonShader    :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CCartoonShader();
    virtual ~CCartoonShader();

public:
    virtual bool Init();
};

