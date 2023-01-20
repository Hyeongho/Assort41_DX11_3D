#include "AnimationMesh.h"
#include "../../PathManager.h"
#include "FBXLoader.h"
#include "../Animation/Skeleton.h"
#include "../Animation/AnimationSequence.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../Shader/StructuredBuffer.h"

CAnimationMesh::CAnimationMesh()	:
	m_InstancingCount(100),
	m_BoneBuffer(nullptr)
{
    m_MeshType = MeshType::Animation;

    SetTypeID<CAnimationMesh>();
}

CAnimationMesh::~CAnimationMesh()
{
	SAFE_DELETE(m_BoneBuffer)
}

int CAnimationMesh::GetBoneCount() const
{
	return (int)m_Skeleton->GetBoneCount();
}

void CAnimationMesh::SetBoneShader()
{
	m_BoneBuffer->SetShader(12, (int)EShaderBufferType::Vertex);
}

void CAnimationMesh::ResetBoneShader()
{
	m_BoneBuffer->ResetShader(12, (int)EShaderBufferType::Vertex);
}

void CAnimationMesh::SetSkeleton(CSkeleton* Skeleton)
{
	m_Skeleton = Skeleton;

	if (!m_BoneBuffer)
		m_BoneBuffer = new CStructuredBuffer;

	m_BoneBuffer->Init("OutputBone", sizeof(Matrix),
		(unsigned int)m_Skeleton->GetBoneCount() * m_InstancingCount, 2);
}

void CAnimationMesh::SetSkeleton(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	TCHAR   FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		lstrcpy(FullPath, Info->Path);

	lstrcat(FullPath, FileName);

	return SetSkeletonFullPath(Name, FullPath);
}

void CAnimationMesh::SetSkeletonFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	char   _FullPath[MAX_PATH] = {};

	int Length = WideCharToMultiByte(CP_ACP, 0, FullPath, -1,
		nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, _FullPath,
		Length, nullptr, nullptr);

	return SetSkeletonMultibyteFullPath(Name, _FullPath);
}

void CAnimationMesh::SetSkeletonMultibyte(const std::string& Name,
	const char* FileName, const std::string& PathName)
{
	char   FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	return SetSkeletonMultibyteFullPath(Name, FullPath);
}

void CAnimationMesh::SetSkeletonMultibyteFullPath(
	const std::string& Name, const char* FullPath)
{
	m_Skeleton = new CSkeleton;

	m_Skeleton->LoadSkeletonFullPath(m_Scene, Name, FullPath);

	if (!m_BoneBuffer)
		m_BoneBuffer = new CStructuredBuffer;

	m_BoneBuffer->Init("OutputBone", sizeof(Matrix),
		(unsigned int)m_Skeleton->GetBoneCount() * m_InstancingCount, 2);
}

void CAnimationMesh::ResizeBoneBuffer(int Count)
{
	m_InstancingCount = Count;

	m_BoneBuffer->Init("OutputBone", sizeof(Matrix),
		(unsigned int)m_Skeleton->GetBoneCount() * m_InstancingCount, 2);
}

bool CAnimationMesh::LoadMesh(const std::string& Name,
    const TCHAR* FileName, const std::string& PathName)
{
    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

    if (Info)
        lstrcpy(FullPath, Info->Path);

    lstrcat(FullPath, FileName);

    return LoadMeshFullPath(Name, FullPath);
}

bool CAnimationMesh::LoadMeshFullPath(const std::string& Name, 
    const TCHAR* FullPath)
{
    char   _FullPath[MAX_PATH] = {};

    int Length = WideCharToMultiByte(CP_ACP, 0, FullPath, -1,
        nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, FullPath, -1, _FullPath,
        Length, nullptr, nullptr);

    return LoadMeshMultibyteFullPath(Name, _FullPath);
}

bool CAnimationMesh::LoadMeshMultibyte(const std::string& Name, 
    const char* FileName, const std::string& PathName)
{
    char   FullPath[MAX_PATH] = {};

    const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

    if (Info)
        strcpy_s(FullPath, Info->PathMultibyte);

    strcat_s(FullPath, FileName);

    return LoadMeshMultibyteFullPath(Name, FullPath);
}

bool CAnimationMesh::LoadMeshMultibyteFullPath(
    const std::string& Name, const char* FullPath)
{
    // FBX 파일인지를 판단한다.
    char    Ext[_MAX_EXT] = {};

    _splitpath_s(FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);
    _strupr_s(Ext);

    if (strcmp(Ext, ".FBX") == 0)
    {
        CFBXLoader  Loader;

        if (!Loader.LoadFBX(FullPath, false))
            return false;

        return ConvertFBX(&Loader, FullPath);
    }

    return LoadMeshFile(FullPath);
}

bool CAnimationMesh::ConvertFBX(CFBXLoader* Loader, 
    const char* FullPath)
{
    if (!CMesh::ConvertFBX(Loader, FullPath))
        return false;

	// 애니메이션 처리
	const std::vector<FbxBone*>* pvecBone = Loader->GetBones();

	if (pvecBone->empty())
		return true;

	std::string	SkeletonName = m_Name + "_Skeleton";

	m_Skeleton = new CSkeleton;

	// 본 수만큼 반복한다.
	std::vector<FbxBone*>::const_iterator	iterB;
	std::vector<FbxBone*>::const_iterator	iterBEnd = pvecBone->end();

	for (iterB = pvecBone->begin(); iterB != iterBEnd; iterB++)
	{
		Bone* pBone = new Bone;

		pBone->Name = (*iterB)->Name;
		pBone->Depth = (*iterB)->Depth;
		pBone->ParentIndex = (*iterB)->ParentIndex;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matOffset[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matBone[i][j] = (float)(*iterB)->matBone.mData[i].mData[j];
			}
		}

		m_Skeleton->AddBone(pBone);
	}

	// 애니메이션 클립을 추가한다.
	const std::vector<FbxAnimationClip*>* pvecClip = Loader->GetClips();

	// 파일 이름을 제외한 경로를 만들어둔다.
	char	AnimPath[MAX_PATH] = {};
	strcpy_s(AnimPath, FullPath);

	int	iLength = (int)strlen(AnimPath);
	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
		if (AnimPath[i] == '/' || AnimPath[i] == '\\')
		{
			memset(&AnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			break;
		}
	}

	// 클립을 읽어온다.
	std::vector<FbxAnimationClip*>::const_iterator	iterC;
	std::vector<FbxAnimationClip*>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; iterC++)
	{
		if (m_Scene)
			m_Scene->GetResource()->LoadAnimationSequence((*iterC)->Name, false, *iterC);

		else
			CResourceManager::GetInst()->LoadAnimationSequence((*iterC)->Name, false, *iterC);

		CAnimationSequence* Sequence = nullptr;
		
		if (m_Scene)
			Sequence = m_Scene->GetResource()->FindAnimationSequence((*iterC)->Name);

		else
			Sequence = CResourceManager::GetInst()->FindAnimationSequence((*iterC)->Name);

		if (!Sequence)
			continue;

		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, FullPath);
		memcpy(&strAnimFullPath[iLength - 3], "sqc", 3);

		Sequence->SaveFullPathMultibyte(strAnimFullPath);
	}

	if (m_Skeleton)
	{
		char	SkeletonPath[MAX_PATH] = {};
		strcpy_s(SkeletonPath, FullPath);
		memcpy(&SkeletonPath[iLength - 3], "bne", 3);
		m_Skeleton->SaveSkeletonFullPath(SkeletonPath);


		if (!m_BoneBuffer)
			m_BoneBuffer = new CStructuredBuffer;

		m_BoneBuffer->Init("OutputBone", sizeof(Matrix),
			(unsigned int)m_Skeleton->GetBoneCount() * m_InstancingCount, 2);
	}

    return true;
}

bool CAnimationMesh::SaveMesh(FILE* File)
{
	if (!CMesh::SaveMesh(File))
		return false;

	return true;
}

bool CAnimationMesh::LoadMesh(FILE* File)
{
	if (!CMesh::LoadMesh(File))
		return false;

	return true;
}
