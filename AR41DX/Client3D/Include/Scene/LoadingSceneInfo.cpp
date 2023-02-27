#include "LoadingSceneInfo.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/LoadingUI.h"
#include "Thread/ThreadManager.h"
#include "Thread/LoadingThread.h"
#include "Thread/ThreadQueue.h"
#include "Thread/DataStream.h"
#include "Editor/EditorGUIManager.h"
#include "Component/ParticleComponent.h"
#include "GameObject/GameObject.h"

//김범중 클라이언트에서는 에디터 관련된 부분 컴파일 안돼게끔 하기위한 전처리문
#ifdef __has_include
#	if __has_include("../Window/ObjectWindow.h")
#		include "../Window/ObjectWindow.h"
#	endif
#endif
//

CLoadingSceneInfo::CLoadingSceneInfo()
	: m_LoadingThread(nullptr)
	, m_LoadingQueue(nullptr)
	, m_LoadingUI(nullptr)
	, m_StartTime(1.f)
{
	m_ClassTypeName = "LoadingSceneInfo";
}

CLoadingSceneInfo::~CLoadingSceneInfo()
{
	m_Owner->GetResource()->SoundStop("LoadingUI");
}

bool CLoadingSceneInfo::Init()
{
	m_LoadingUI = m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");

    return true;
}

void CLoadingSceneInfo::Update(float DeltaTime)
{
	CSceneInfo::Update(DeltaTime);
	m_StartTime -= DeltaTime;
	if (m_StartTime < 0.f)
	{
		m_Owner->GetResource()->SoundPlay("LoadingUI");
		CGameObject* obj = m_Owner->CreateObject<CGameObject>("Particle");
		CParticleComponent* particle = obj->CreateComponent<CParticleComponent>("Particle");
		particle->SetParticle("LoadingBubble");
		obj->SetLifeTime(1.5f);
		m_StartTime = 100.f;
	}

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

		//m_LoadingUI->SetLoadingPercent(Rate);
	}

	if (m_LoadingThread->IsLoadComplete())
	{
		CScene* nextScene = CSceneManager::GetInst()->GetNextScene();
		nextScene->GetSceneInfo()->SetFileName(m_FileName);
		nextScene->GetSceneInfo()->SetPrevFileName(m_PrevFileName);
		CSceneManager::GetInst()->ChangeNextScene();
		CThreadManager::GetInst()->Delete("Loading");
#if __has_include("../Window/ObjectWindow.h")
		CObjectWindow* objectWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
		if (objectWindow)
		{
			objectWindow->Clear();
			objectWindow->ClearSelect();
			objectWindow->AddInput(nextScene);
			objectWindow->AddItemList(nextScene);
		}
#endif
	}
}

void CLoadingSceneInfo::SceneChangeComplete()
{
	// 로딩 스레드 생성
	m_LoadingThread = CThreadManager::GetInst()->Create<CLoadingThread>("Loading");

	m_LoadingThread->SetLoadingSceneFileName(m_FileName);

	m_LoadingQueue = m_LoadingThread->GetQueue();

	m_LoadingThread->Start();
}
