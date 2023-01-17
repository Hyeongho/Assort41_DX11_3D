#pragma once

#include "AnimationSequence2D.h"
#include "AnimationSequence.h"
#include "Skeleton.h"
#include "../Mesh/FBXLoader.h"

class CAnimationManager
{
	friend class CResourceManager;

private:
	CAnimationManager();
	~CAnimationManager();

private:
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapSequence2D;
	class CAnimation2DConstantBuffer* m_Anim2DBuffer;

	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>>	m_mapSequence;
	std::unordered_map<std::string, CSharedPtr<CSkeleton>>	m_mapSkeleton;

public:
	class CAnimation2DConstantBuffer* GetAnim2DConstantBuffer()	const
	{
		return m_Anim2DBuffer;
	}

public:
	bool Init();
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& Name, class CTexture* Texture);
	bool CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const TCHAR* FullPath);
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFullPath);
	bool AddSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& End);
	bool AddSequence2DFrame(const std::string& Name, float StartX, float StartY, float EndX,
		float EndY);
	bool AddSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End);
	bool AddSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX,
		float EndY);
	bool SaveSequence2D(const std::string& Name, const char* FullPath);
	bool LoadSequence2D(const std::string& Name, const char* FullPath);
	bool SaveSequence2D(const std::string& Name, const char* FileName, const std::string& PathName);
	bool LoadSequence2D(const std::string& Name, const char* FileName, const std::string& PathName);

	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	void ReleaseAnimationSequence2D(const std::string& Name);




public:	// Animation 3D
	bool LoadAnimationSequence(const std::string& Name, bool Loop,
		const FbxAnimationClip* Clip);
	bool LoadAnimationSequence(const std::string& Name, bool Loop,
		int StartFrame, int EndFrame, float PlayTime,
		const std::vector<BoneKeyFrame*>& vecFrame);
	bool LoadAnimationSequence(const std::string& Name, 
		const TCHAR* FileName, const std::string& PathName = ANIMATION_PATH);
	bool LoadAnimationSequenceFullPath(const std::string& Name, 
		const TCHAR* FullPath);
	bool LoadAnimationSequenceMultibyte(const std::string& Name, 
		const char* FileName, const std::string& PathName = ANIMATION_PATH);
	bool LoadAnimationSequenceMultibyteFullPath(const std::string& Name, 
		const char* FullPath);

	CAnimationSequence* FindAnimationSequence(const std::string& Name);
	void ReleaseAnimationSequence(const std::string& Name);


	bool LoadSkeleton(class CScene* Scene, const std::string& Name,
		const TCHAR* FileName, const std::string& PathName = ANIMATION_PATH);
	bool LoadSkeletonFullPath(class CScene* Scene, const std::string& Name,
		const TCHAR* FullPath);
	bool LoadSkeletonMultibyte(class CScene* Scene, const std::string& Name,
		const char* FileName, const std::string& PathName = ANIMATION_PATH);
	bool LoadSkeletonMultibyteFullPath(class CScene* Scene, const std::string& Name,
		const char* FullPath);
	void AddSocket(const std::string& SkeletonName,
		const std::string& BoneName, const std::string& SocketName,
		const Vector3& Offset, const Vector3& OffsetRot);

	CSkeleton* FindSkeleton(const std::string& Name);
	void ReleaseSkeleton(const std::string& Name);
};

