#pragma once

#include "GraphicShader.h"

class CTileMapBackShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CTileMapBackShader();
    virtual ~CTileMapBackShader();

public:
    virtual bool Init();
};

