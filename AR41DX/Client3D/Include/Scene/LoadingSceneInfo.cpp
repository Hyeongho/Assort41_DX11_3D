#include "LoadingSceneInfo.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/LoadingUI.h"
#include "Thread/ThreadManager.h"
#include "Thread/LoadingThread.h"
#include "Thread/ThreadQueue.h"
#include "Thread/DataStream.h"

CLoadingSceneInfo::CLoadingSceneInfo()
{
}

CLoadingSceneInfo::~CLoadingSceneInfo()
{
}

bool CLoadingSceneInfo::Init()
{
	m_LoadingUI = m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");

    return true;
}

void CLoadingSceneInfo::Update(float DeltaTime)
{
	CSceneInfo::Update(DeltaTime);

	// Queue에 데이터가 있을 경우 받아와서 처리한다.
	if (!m_LoadingQueue->empty())
	{
		int	Header, Size;
		unsigned char	Data[1024] = {};

		m_LoadingQueue->pop(&Header, &Size, Data);

		CDataStream	stream;
		stream.SetBuffer(Data);

		float	Rate = 0.f;

		stream.GetData<float>(&Rate, 4);

		m_LoadingUI->SetLoadingPercent(Rate);
	}

	if (m_LoadingThread->IsLoadComplete())
	{
		CSceneManager::GetInst()->ChangeNextScene();

		CThreadManager::GetInst()->Delete("Loading");
	}
}

void CLoadingSceneInfo::SceneChangeComplete()
{
	// 로딩 스레드 생성
	m_LoadingThread = CThreadManager::GetInst()->Create<CLoadingThread>("Loading");

	m_LoadingThread->SetLoadingSceneFileName("Main2.scn");
	//m_LoadingThread->SetLoadingScenePathName(SCENE_PATH);

	m_LoadingQueue = m_LoadingThread->GetQueue();

	m_LoadingThread->Start();

	//CSceneManager::GetInst()->GetNextScene()->Load("Main");
}
