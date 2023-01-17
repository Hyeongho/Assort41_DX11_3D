#pragma once

#include "GraphicShader.h"

class CColliderImageShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CColliderImageShader();
    virtual ~CColliderImageShader();

public:
    virtual bool Init();
};

