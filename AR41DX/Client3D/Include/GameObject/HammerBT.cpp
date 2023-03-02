#include "HammerBT.h"
#include "../GameObject/AI/Sequence.h"
#include "../GameObject/AI/Selector.h"
#include "Hammer.h"
#include "../GameObject/Walk.h"
#include "../GameObject/Chase.h"
#include "../GameObject/Attack.h"
#include "../GameObject/Dead.h"

CHammerBT::CHammerBT()
{
}

CHammerBT::~CHammerBT()
{
}

void CHammerBT::Start()
{
	m_Root = new CSequence;

	m_Sequence = new CSequence;

	m_Walk = new CWalk;
	m_Chase = new CChase;
	m_Attack = new CAttack;
	m_Dead = new CDead;

	m_Root->AddChild(m_Sequence);

	m_Sequence->AddChild(m_Walk);
	m_Sequence->AddChild(m_Chase);
	m_Sequence->AddChild(m_Attack);
	m_Sequence->AddChild(m_Dead);
}

void CHammerBT::Run()
{
	m_Root->Run();
}

void CHammerBT::Run(CHammer* Hammer)
{
	m_Root->Run(Hammer);
}

