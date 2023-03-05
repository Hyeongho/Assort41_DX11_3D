
#pragma once

#include "../GameObject/AI/Node.h"

class CDead :
    public CNode
{
    friend class CScene;

public:
    CDead();
    ~CDead();

private:
    class CScene* m_Scene;
    class CFodder* m_Fodder;
    class CHammer* m_Hammer;

public:
    virtual bool    Run();
    virtual bool    Run(CGameObject* Object);
};

