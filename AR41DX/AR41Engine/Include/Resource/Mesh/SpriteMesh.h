#pragma once

#include "Mesh.h"

class CSpriteMesh :
    public CMesh
{
    friend class CMeshManager;

protected:
    CSpriteMesh();
    virtual ~CSpriteMesh();
};
