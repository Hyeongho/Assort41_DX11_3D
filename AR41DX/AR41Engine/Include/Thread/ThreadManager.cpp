#include "ThreadManager.h"
#include "../Component/SceneComponent.h"
#include "../Scene/Scene.h"
#include "Navigation2DThread.h"
#include "Navigation3DThread.h"

DEFINITION_SINGLE(CThreadManager)

CThreadManager::CThreadManager()
{
}

CThreadManager::~CThreadManager()
{
	{
		auto iter = m_mapThread.begin();
		auto iterEnd = m_mapThread.end();

		for (; iter != iterEnd; iter++)
		{
			iter->second->Stop();
			SAFE_DELETE(iter->second);
		}

		m_mapThread.clear();
	}

	{
		auto	iter = m_mapCriticalSection.begin();
		auto	iterEnd = m_mapCriticalSection.end();

		for (; iter != iterEnd; iter++)
		{
			// 전역함수를 호출하겠다는 의미이다.
			::DeleteCriticalSection(iter->second);

			SAFE_DELETE(iter->second);
		}

		m_mapCriticalSection.clear();
	}
}

bool CThreadManager::Init()
{
	// 로딩용 크리티컬 섹션 생성
	CreateCriticalSection("Loading");

	return true;
}

bool CThreadManager::Suspend(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
	{
		return false;
	}

	Thread->Suspend();

	return true;
}

bool CThreadManager::Resume(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
	{
		return false;
	}

	Thread->Resume();

	return true;
}

bool CThreadManager::ReStart(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
	{
		return false;
	}

	Thread->ReStart();

	return true;
}

bool CThreadManager::Delete(const std::string& Name)
{
	auto iter = m_mapThread.find(Name);

	if (iter == m_mapThread.end())
	{
		return false;
	}

	iter->second->Stop();
	SAFE_DELETE(iter->second);

	m_mapThread.erase(iter);

	return true;
}

bool CThreadManager::Start(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
	{
		return false;
	}

	Thread->Start();

	return true;
}

void CThreadManager::CreateNavigationThread(CSceneComponent* NavComponent, bool Is2D)
{
	CScene* Scene = NavComponent->GetScene();

	unsigned __int64 Address = (unsigned __int64)Scene;

	char SceneAddress[32] = {};

	sprintf_s(SceneAddress, "%llu", Address);

	std::string	Name = Scene->GetName();
	Name += "_";
	Name += NavComponent->GetName();
	Name += "_";
	Name += SceneAddress;

	SYSTEM_INFO	SysInfo = {};

	GetSystemInfo(&SysInfo);

	for (DWORD i = 0; i < SysInfo.dwNumberOfProcessors * 2; i++)
	{
		char ThreadName[256] = {};

		sprintf_s(ThreadName, "%s_%d", Name.c_str(), (int)i);

		CNavigationThread* Thread = nullptr;

		if (Is2D)
		{
			Thread = Create<CNavigation2DThread>(ThreadName);
		}

		else
		{
			Thread = Create<CNavigation3DThread>(ThreadName);
		}

		Scene->GetNavigationManager()->AddNavigationThread(Thread);

		Thread->SetNavigationComponent(NavComponent);
		Thread->SetLoop(true);

		Thread->Start();

		Thread->Suspend();
	}
}

void CThreadManager::DeleteNavigationThread(CSceneComponent* NavComponent)
{
	CScene* Scene = NavComponent->GetScene();

	if (!Scene)
	{
		return;
	}

	unsigned __int64 Address = (unsigned __int64)Scene;

	char SceneAddress[32] = {};

	sprintf_s(SceneAddress, "%llu", Address);

	std::string	Name = NavComponent->GetSceneName();
	Name += "_";
	Name += NavComponent->GetName();
	Name += "_";
	Name += SceneAddress;

	SYSTEM_INFO	SysInfo = {};

	GetSystemInfo(&SysInfo);

	for (DWORD i = 0; i < SysInfo.dwNumberOfProcessors * 2; i++)
	{
		char ThreadName[256] = {};

		sprintf_s(ThreadName, "%s_%d", Name.c_str(), (int)i);

		CThread* Thread = FindThread(ThreadName);

		if (!Thread)
		{
			continue;
		}

		Thread->ReStart();

		Delete(ThreadName);
	}
}

CThread* CThreadManager::FindThread(const std::string& Name)
{
	auto iter = m_mapThread.find(Name);

	if (iter == m_mapThread.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool CThreadManager::CreateCriticalSection(const std::string& Name)
{
	CRITICAL_SECTION* Crt = FindCriticalSection(Name);

	if (Crt)
	{
		return false;
	}

	Crt = new CRITICAL_SECTION;

	InitializeCriticalSection(Crt);

	m_mapCriticalSection.insert(std::make_pair(Name, Crt));

	return true;
}

bool CThreadManager::DeleteCriticalSection(const std::string& Name)
{
	auto iter = m_mapCriticalSection.find(Name);

	if (iter == m_mapCriticalSection.end())
	{
		return false;
	}

	::DeleteCriticalSection(iter->second);

	SAFE_DELETE(iter->second);

	m_mapCriticalSection.erase(iter);

	return true;
}

CRITICAL_SECTION* CThreadManager::FindCriticalSection(const std::string& Name)
{
	auto iter = m_mapCriticalSection.find(Name);

	if (iter == m_mapCriticalSection.end())
	{
		return nullptr;
	}

	return iter->second;
}
