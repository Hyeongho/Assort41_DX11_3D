#pragma once

#include "CompositeNode.h"

class CSelector :
    public CCompositeNode
{
public:
    virtual bool Run();
    virtual bool Run(class CGameObject* Object);
};

