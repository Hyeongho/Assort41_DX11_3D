#pragma once

#include "Ref.h"

#include "../../AI/Selector.h"
#include "../../AI/Sequence.h"
#include "RoboSpongeAttackHoriz.h"
#include "RoboSpongeAttackVirtic.h"
#include "RoboSpongeAttackWords.h"

class CRoboSpongeBT
	: public CRef
{
	public:
		CRoboSpongeBT();
		virtual ~CRoboSpongeBT();

	private:
		CSharedPtr<CSequence> m_Root;

		CSharedPtr<CSelector> m_Selector1;

		CSharedPtr<CSequence> m_Squence1;
		CSharedPtr<CSequence> m_Squence2;
		CSharedPtr<CSequence> m_Squence3;

		//CSharedPtr<CRoboSpongeAttackHoriz> m_AttackHoriz;
		//CSharedPtr<CRoboSpongeAttackVirtic> m_AttackVirtic;
		//CSharedPtr<CRoboSpongeAttackWords> m_AttackWords;


	public:
		void Start();
		void Run(bool IsRun = true);
};

