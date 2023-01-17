#pragma once

#include "Thread.h"
#include "ThreadQueue.h"

enum class ELoadingHeader
{
	LoadingRate
};

class CLoadingThread :
    public CThread
{
	friend class CThreadManager;

protected:
	CLoadingThread();
	virtual ~CLoadingThread();

protected:
	std::string		m_FileName;
	std::string		m_PathName;
	std::string		m_FullPath;
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
	void SetLoadingSceneFileName(const std::string& FileName)
	{
		m_FileName = FileName;
	}

	void SetLoadingScenePathName(const std::string& PathName)
	{
		m_PathName = PathName;
	}

	void SetLoadingSceneFullPath(const std::string& FullPath)
	{
		m_FullPath = FullPath;
	}

public:
	virtual void Run();

private:
	void LoadingCallback(float Rate);
};

