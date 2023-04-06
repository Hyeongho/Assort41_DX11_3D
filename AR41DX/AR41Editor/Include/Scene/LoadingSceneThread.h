#pragma once

#include "Thread/Thread.h"
#include "Thread/ThreadQueue.h"

enum class ELoadingSceneHeader
{
	LoadingRate
};

class CLoadingSceneThread :
	public CThread
{
	friend class CThreadManager;

protected:
	CLoadingSceneThread();
	virtual ~CLoadingSceneThread();

protected:
	std::string		m_NextScene;
	std::string		m_PathName;
	bool			m_LoadComplete;
	CThreadQueue	m_Queue;

public:
	CThreadQueue* GetQueue()
	{
		return &m_Queue;
	}

	bool IsLoadComplete()	const
	{
		return m_LoadComplete;
	}

public:
	void SetLoadingSceneNextSceneName(const std::string& NextScene)
	{
		m_NextScene = NextScene;
	}

	void SetLoadingScenePathName(const std::string& PathName)
	{
		m_PathName = PathName;
	}

public:
	virtual void Run();

private:
	void LoadingCallback(float Rate);
};

