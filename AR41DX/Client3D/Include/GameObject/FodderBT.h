#pragma once

#include "Ref.h"

class CFodderBT :
	public CRef
{
public:
	CFodderBT();
	virtual ~CFodderBT();

private:
	CSharedPtr<class CSequence> m_Root;
	CSharedPtr<class CSequence> m_Sequence;

	CSharedPtr<class CWalk>		m_Walk;
	CSharedPtr<class CChase>    m_Chase;
	CSharedPtr<class CAttack>	m_Attack;
	CSharedPtr<class CDead>		m_Dead;

public:
	void Start();
	void Run();
	void Run(CFodder* Fodder);
};

