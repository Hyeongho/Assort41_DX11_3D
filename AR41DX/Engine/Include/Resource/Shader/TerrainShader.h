#pragma once

#include "GraphicShader.h"

class CTerrainShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CTerrainShader();
    virtual ~CTerrainShader();

public:
    virtual bool Init();
};

