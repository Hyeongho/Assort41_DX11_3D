#include "RoboSpongeBT.h"

CRoboSpongeBT::CRoboSpongeBT()
{
}

CRoboSpongeBT::~CRoboSpongeBT()
{
}

void CRoboSpongeBT::Start()
{
	//m_Root = new CSequence;
	//
	//m_Selector1 = new CSelector;

	//m_Squence1 = new CSequence;
	//m_Squence2 = new CSequence;
	//m_Squence3 = new CSequence;


	//m_Root->AddChild(m_Selector1);

	//m_Selector1->AddChild(m_Squence1);
	//m_Selector1->AddChild(m_Squence2);
	//m_Selector1->AddChild(m_Squence3);


}

void CRoboSpongeBT::Run(bool IsRun)
{
	if (IsRun)
	{
		m_Root->Run();
	}
}
