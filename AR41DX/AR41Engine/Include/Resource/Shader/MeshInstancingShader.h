#pragma once
#include "GraphicShader.h"
class CMeshInstancingShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CMeshInstancingShader();
    virtual ~CMeshInstancingShader();

public:
    virtual bool Init();
};

