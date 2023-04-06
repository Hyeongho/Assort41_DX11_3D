#include "LoadingSceneThread.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "PathManager.h"
#include "Thread/DataStream.h"
#include "BikiniCitySceneInfo.h"
#include "RoboSpongeSceneInfo.h"
#include "JellyfishField1SceneInfo.h"
#include "JellyfishField2SceneInfo.h"
#include "KingJellyfishSceneInfo.h"
#include "TitleSceneInfo.h"

CLoadingSceneThread::CLoadingSceneThread() :
	m_LoadComplete(false)
{
	m_PathName = SCENE_PATH;
}

CLoadingSceneThread::~CLoadingSceneThread()
{
}

void CLoadingSceneThread::Run()
{
	// Main Scene »ý¼º
	CSceneManager::GetInst()->CreateNextScene(false);

	if (m_NextScene == "TitleScene")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CTitleSceneInfo>(false);
	}
	
	else if (m_NextScene == "BikiniCity")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CBikiniCitySceneInfo>(false);
	}

	else if (m_NextScene == "JellyFishField1")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CJellyfishField1SceneInfo>(false);
	}

	else if (m_NextScene == "JellyFishField2")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CJellyfishField2SceneInfo>(false);
	}

	else if (m_NextScene == "KingJellyfish")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CKingJellyfishSceneInfo>(false);
	}

	else if (m_NextScene == "ChumBucketLab")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CRoboSpongeSceneInfo>(false);
	}

	CSceneManager::GetInst()->GetNextScene()->SetLoadingCallback<CLoadingSceneThread>(this, &CLoadingSceneThread::LoadingCallback);

	CSceneManager::GetInst()->GetNextScene()->Init();

	m_LoadComplete = true;
}

void CLoadingSceneThread::LoadingCallback(float Rate)
{
	unsigned char	Buffer[1024] = {};

	CDataStream	stream;
	stream.SetBuffer(Buffer);

	stream.AddData<float>(&Rate, 4);

	m_Queue.push((int)ELoadingSceneHeader::LoadingRate, stream.GetSize(),
		Buffer);

	//Sleep(200);
}
