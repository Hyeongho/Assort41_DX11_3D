#pragma once

#include "GraphicShader.h"

class CDropShader :
	public CGraphicShader
{
    friend class CShaderManager;

protected:
    CDropShader();
    virtual ~CDropShader();

public:
    virtual bool Init();
};

