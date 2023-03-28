
#pragma once

#include "GraphicShader.h"

class CMSShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CMSShader();
    virtual ~CMSShader();

public:
    virtual bool Init();
};

