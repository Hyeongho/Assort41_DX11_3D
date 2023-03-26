#pragma once

#include "GraphicShader.h"

class CTranslationShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CTranslationShader();
    virtual ~CTranslationShader();

public:
    virtual bool Init();
};

