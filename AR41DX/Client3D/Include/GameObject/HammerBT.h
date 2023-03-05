
#pragma once

#include "Ref.h"

class CHammerBT	:
	public CRef
{
public:
	CHammerBT();
	virtual ~CHammerBT();

private:
	CSharedPtr<class CSequence> m_Root;
	CSharedPtr<class CSequence> m_Sequence;

	CSharedPtr<class CWalk>	m_Walk;
	CSharedPtr<class CChase>  m_Chase;
	CSharedPtr<class CAttack>	m_Attack;
	CSharedPtr<class CDead>	m_Dead;

	//class CWalk* m_Walk;
	//class CChase* m_Chase;
	//class CAttack* m_Attack;
	//class CDead* m_Dead;

public:
	void Start();
	void Run();
	void Run(CHammer* Hammer);
};

