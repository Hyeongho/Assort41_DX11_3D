
#pragma once

#include "../GameObject/AI/Node.h"

class CAttack :
    public CNode
{
    friend class CScene;
    friend class CFodder;

public:
    CAttack();
    ~CAttack();

private:
    class CFodder* m_Fodder;
    class CHammer* m_Hammer;

public:
    virtual bool    Run();
    virtual bool    Run(CGameObject* Object);
};

