
#pragma once

#include "../GameObject/AI/Node.h"

class CChase :
    public CNode
{
    friend class CScene;
    friend class CFodder;

public:
    CChase();
    ~CChase();

private:
    class CScene* m_Scene;
    class CFodder* m_Fodder;
    class CHammer* m_Hammer;

public:
    virtual bool    Run(); 
    virtual bool    Run(CGameObject* Object);
};

