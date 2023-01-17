#pragma once
#include "GraphicShader.h"
class CMeshShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CMeshShader();
    virtual ~CMeshShader();

public:
    virtual bool Init();
};

