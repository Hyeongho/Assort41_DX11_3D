#pragma once
#include "GraphicShader.h"
class CScreenShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CScreenShader();
    virtual ~CScreenShader();

public:
    virtual bool Init();
};

