#pragma once

#include "../GameObject/AI/Node.h"

class CWalk :
    public CNode
{
    friend class CScene;

public:
    CWalk();
    ~CWalk();

private:
    class CScene* m_Scene;
    class CFodder* m_Fodder;
    class CHammer* m_Hammer;

public: 
    virtual bool    Run();
    virtual bool    Run(CGameObject* Object);
};

