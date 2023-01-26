#include "MainSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Weapon.h"
#include "../GameObject/Sandy.h"

CMainSceneInfo::CMainSceneInfo()
{
}

CMainSceneInfo::~CMainSceneInfo()
{
}

bool CMainSceneInfo::Init()
{
	CSceneInfo::Init();

	CSandy* Sandy = m_Owner->CreateObject<CSandy>("Sandy");

	SetPlayerObject(Sandy);

	//CWeapon* Weapon = m_Owner->CreateObject<CWeapon>("Weapon");

	//Player->AddChildToSocket("Weapon", Weapon);

	/*for (int i = 0; i < 100; ++i)
	{
		CMonster* Monster = m_Owner->CreateObject<CMonster>("Monster");

		Monster->SetWorldPosition(-250.f + i * 4.f, 0.f, 50.f);
	}*/

	/*CMonster* Monster = m_Owner->CreateObject<CMonster>("Monster");

	Monster->SetWorldPosition(-30.f, 0.f, 50.f);

	Monster = m_Owner->CreateObject<CMonster>("Monster");

	Monster->SetWorldPosition(-150.f, 0.f, -20.f);

	Monster = m_Owner->CreateObject<CMonster>("Monster");

	Monster->SetWorldPosition(30.f, 0.f, 50.f);*/

	//Monster = m_Owner->CreateObject<CMonster>("Monster");

	//Monster->SetWorldPosition(150.f, 0.f, -20.f);

	//CMonster* Monster = m_Owner->CreateObject<CMonster>("Monster");


	return true;
}
