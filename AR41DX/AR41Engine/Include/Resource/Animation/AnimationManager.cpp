
#include "AnimationManager.h"
#include "../Shader/Animation2DConstantBuffer.h"

CAnimationManager::CAnimationManager()	:
	m_Anim2DBuffer(nullptr)
{
}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(m_Anim2DBuffer);
}

bool CAnimationManager::Init()
{
	m_Anim2DBuffer = new CAnimation2DConstantBuffer;

	m_Anim2DBuffer->Init();

	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& Name, 
	const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence2D;

	Sequence->SetName(Name);

	if (!Sequence->Init(TextureName, FileName, PathName))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& Name, 
	CTexture* Texture)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence2D;

	Sequence->SetName(Name);

	if (!Sequence->Init(Texture))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::CreateAnimationSequence2DFullPath(
	const std::string& Name, const std::string& TextureName,
	const TCHAR* FullPath)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence2D;

	Sequence->SetName(Name);

	if (!Sequence->InitFullPath(TextureName, FullPath))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(
	const std::string& Name, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFileName, 
	const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence2D;

	Sequence->SetName(Name);

	if (!Sequence->Init(TextureName, vecFileName, PathName))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::CreateAnimationSequence2DFullPath(
	const std::string& Name, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence2D;

	Sequence->SetName(Name);

	if (!Sequence->InitFullPath(TextureName, vecFullPath))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::AddSequence2DFrame(const std::string& Name, const Vector2& Start,
	const Vector2& End)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrame(Start, End);

	return true;
}

bool CAnimationManager::AddSequence2DFrame(const std::string& Name, float StartX,
	float StartY, float EndX, float EndY)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrame(StartX, StartY, EndX, EndY);

	return true;
}

bool CAnimationManager::AddSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrameAll(Count, Start, End);

	return true;
}

bool CAnimationManager::AddSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX, float EndY)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrameAll(Count, StartX, StartY, EndX, EndY);

	return true;
}

bool CAnimationManager::SaveSequence2D(const std::string& Name, const char* FullPath)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	return Sequence->Save(FullPath);
}

bool CAnimationManager::LoadSequence2D(const std::string& Name, const char* FullPath)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	bool	Find = true;

	if (!Sequence)
	{
		Sequence = new CAnimationSequence2D;

		Sequence->SetName(Name);

		Find = false;
	}

	if (!Sequence->Load(FullPath))
		return false;

	if(!Find)
		m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::SaveSequence2D(const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	return Sequence->Save(FileName, PathName);
}

bool CAnimationManager::LoadSequence2D(const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	bool	Find = true;

	if (!Sequence)
	{
		Sequence = new CAnimationSequence2D;

		Sequence->SetName(Name);

		Find = false;
	}

	if (!Sequence->Load(FileName, PathName))
		return false;

	if (!Find)
		m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

CAnimationSequence2D* CAnimationManager::FindAnimationSequence2D(
	const std::string& Name)
{
	auto	iter = m_mapSequence2D.find(Name);
	
	if (iter == m_mapSequence2D.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::ReleaseAnimationSequence2D(const std::string& Name)
{
	auto	iter = m_mapSequence2D.find(Name);

	if (iter != m_mapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSequence2D.erase(iter);
	}
}

bool CAnimationManager::LoadAnimationSequence(
	const std::string& Name, bool Loop, 
	const FbxAnimationClip* Clip)
{
	CAnimationSequence* Sequence = FindAnimationSequence(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence;

	Sequence->SetName(Name);

	if (!Sequence->CreateSequence(Loop, Clip))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::LoadAnimationSequence(
	const std::string& Name, bool Loop, int StartFrame, 
	int EndFrame, float PlayTime, 
	const std::vector<BoneKeyFrame*>& vecFrame)
{
	CAnimationSequence* Sequence = FindAnimationSequence(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence;

	Sequence->SetName(Name);

	if (!Sequence->CreateSequence(Name, Loop, StartFrame,
		EndFrame, PlayTime, vecFrame))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::LoadAnimationSequence(
	const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence;

	Sequence->SetName(Name);

	if (!Sequence->CreateSequence(Name, FileName, PathName))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::LoadAnimationSequenceFullPath(
	const std::string& Name, const TCHAR* FullPath)
{
	CAnimationSequence* Sequence = FindAnimationSequence(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence;

	Sequence->SetName(Name);

	if (!Sequence->CreateSequenceFullPath(Name, FullPath))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::LoadAnimationSequenceMultibyte(
	const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence;

	Sequence->SetName(Name);

	if (!Sequence->CreateSequenceMultibyte(Name, FileName, PathName))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::LoadAnimationSequenceMultibyteFullPath(
	const std::string& Name, const char* FullPath)
{
	CAnimationSequence* Sequence = FindAnimationSequence(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence;

	Sequence->SetName(Name);

	if (!Sequence->CreateSequenceFullPathMultibyte(Name, FullPath))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence.insert(std::make_pair(Name, Sequence));

	return true;
}

CAnimationSequence* CAnimationManager::FindAnimationSequence(
	const std::string& Name)
{
	auto	iter = m_mapSequence.find(Name);

	if (iter == m_mapSequence.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::ReleaseAnimationSequence(
	const std::string& Name)
{
	auto	iter = m_mapSequence.find(Name);

	if (iter != m_mapSequence.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSequence.erase(iter);
	}
}

bool CAnimationManager::LoadSkeleton(CScene* Scene, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	CSkeleton* Skeleton = FindSkeleton(Name);

	if (Skeleton)
		return true;

	Skeleton = new CSkeleton;

	Skeleton->SetName(Name);

	char    FileNameMultibyte[MAX_PATH] = {};

	int Length = WideCharToMultiByte(CP_ACP, 0, FileName,
		-1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FileName,
		-1, FileNameMultibyte, Length, 0, 0);

	if (!Skeleton->LoadSkeleton(Scene, Name, FileNameMultibyte, PathName))
	{
		SAFE_DELETE(Skeleton);
		return false;
	}

	m_mapSkeleton.insert(std::make_pair(Name, Skeleton));

	return true;
}

bool CAnimationManager::LoadSkeletonFullPath(
	CScene* Scene, const std::string& Name, const TCHAR* FullPath)
{
	CSkeleton* Skeleton = FindSkeleton(Name);

	if (Skeleton)
		return true;

	Skeleton = new CSkeleton;

	Skeleton->SetName(Name);

	char    FullPathMultibyte[MAX_PATH] = {};

	int Length = WideCharToMultiByte(CP_ACP, 0, FullPath,
		-1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FullPath,
		-1, FullPathMultibyte, Length, 0, 0);

	if (!Skeleton->LoadSkeletonFullPath(Scene, Name, FullPathMultibyte))
	{
		SAFE_DELETE(Skeleton);
		return false;
	}

	m_mapSkeleton.insert(std::make_pair(Name, Skeleton));

	return true;
}

bool CAnimationManager::LoadSkeletonMultibyte(
	CScene* Scene, const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	CSkeleton* Skeleton = FindSkeleton(Name);

	if (Skeleton)
		return true;

	Skeleton = new CSkeleton;

	Skeleton->SetName(Name);

	if (!Skeleton->LoadSkeleton(Scene, Name, FileName, PathName))
	{
		SAFE_DELETE(Skeleton);
		return false;
	}

	m_mapSkeleton.insert(std::make_pair(Name, Skeleton));

	return true;
}

bool CAnimationManager::LoadSkeletonMultibyteFullPath(
	CScene* Scene, const std::string& Name, const char* FullPath)
{
	CSkeleton* Skeleton = FindSkeleton(Name);

	if (Skeleton)
		return true;

	Skeleton = new CSkeleton;

	Skeleton->SetName(Name);

	if (!Skeleton->LoadSkeletonFullPath(Scene, Name, FullPath))
	{
		SAFE_DELETE(Skeleton);
		return false;
	}

	m_mapSkeleton.insert(std::make_pair(Name, Skeleton));

	return true;
}

void CAnimationManager::AddSocket(const std::string& SkeletonName,
	const std::string& BoneName, const std::string& SocketName, 
	const Vector3& Offset, const Vector3& OffsetRot)
{
	CSkeleton* Skeleton = FindSkeleton(SkeletonName);

	if (!Skeleton)
		return;

	Skeleton->AddSocket(BoneName, SocketName, Offset, OffsetRot);
}

CSkeleton* CAnimationManager::FindSkeleton(const std::string& Name)
{
	auto	iter = m_mapSkeleton.find(Name);

	if (iter == m_mapSkeleton.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::ReleaseSkeleton(const std::string& Name)
{
	auto	iter = m_mapSkeleton.find(Name);

	if (iter != m_mapSkeleton.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSkeleton.erase(iter);
	}
}
