#include "FodderBT.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "../GameObject/AI/Sequence.h"
#include "../GameObject/AI/Selector.h"
#include "Fodder.h"
#include "Walk.h"
#include "Chase.h"
#include "Attack.h"
#include "Dead.h"

CFodderBT::CFodderBT()
{
}

CFodderBT::~CFodderBT()
{
}

void CFodderBT::Start()
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

void CFodderBT::Run()
{
	m_Root->Run();
}

void CFodderBT::Run(CFodder* Fodder)
{
	m_Root->Run(Fodder);
}


