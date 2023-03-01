#pragma once

#include "Ref.h"

class CFodderBT :
    public CRef
{
public:
    CFodderBT();
    virtual ~CFodderBT();

private:
	CSharedPtr<CSequence> m_Root;
	CSharedPtr<CSequence> m_Sequence;

	CSharedPtr<CWalk>	m_Walk;
	CSharedPtr<CChase> m_Chase;
	CSharedPtr<CAttack>	m_Attack;
	CSharedPtr<CDead>	m_Dead;

public:
	void Start();
	void Run();
	void Run(CFodder* Fodder);

};

