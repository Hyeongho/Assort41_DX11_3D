#include "Skeleton.h"
#include "../../Device.h"
#include "../../PathManager.h"
#include "../Shader/StructuredBuffer.h"
#include "SkeletonSocket.h"

CSkeleton::CSkeleton()	:
	m_pOffsetMatrixBuffer(nullptr),
	m_Scene(nullptr)
{
	SetTypeID<CSkeleton>();
}

CSkeleton::CSkeleton(const CSkeleton& Skeleton)
{
	for (size_t i = 0; i < Skeleton.m_vecBones.size(); ++i)
	{
		Bone* pBone = new Bone;

		*pBone = *Skeleton.m_vecBones[i];

		m_vecBones.push_back(pBone);
	}

	m_pOffsetMatrixBuffer = nullptr;

	m_BoneSocket.clear();

	size_t	Size = Skeleton.m_BoneSocket.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CSkeletonSocket* Socket = Skeleton.m_BoneSocket[i]->Clone();

		m_BoneSocket.push_back(Socket);
	}
}

CSkeleton::~CSkeleton()
{
	SAFE_DELETE(m_pOffsetMatrixBuffer);

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		SAFE_DELETE(m_vecBones[i]);
	}

	m_vecBones.clear();

	size_t	Size = m_BoneSocket.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_BoneSocket[i]);
	}

	m_BoneSocket.clear();
}

size_t CSkeleton::GetBoneCount() const
{
	return m_vecBones.size();
}

Bone* CSkeleton::GetBone(int Index) const
{
	return m_vecBones[Index];
}

Bone* CSkeleton::GetBone(const std::string& Name) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->Name == Name)
			return m_vecBones[i];
	}

	return nullptr;
}

int CSkeleton::GetBoneIndex(const std::string& Name) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->Name == Name)
			return (int)i;
	}

	return -1;
}

bool CSkeleton::CheckBone(const std::string& Name) const
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->Name == Name)
			return true;
	}

	return false;
}

const Matrix& CSkeleton::GetBoneMatrix(const std::string& Name) const
{
	int	Index = GetBoneIndex(Name);

	return m_vecBones[Index]->matBone;
}

const Matrix& CSkeleton::GetBoneMatrix(int Index) const
{
	return m_vecBones[Index]->matBone;
}

CSkeletonSocket* CSkeleton::GetSocket(const std::string& Name)
{
	size_t	Size = m_BoneSocket.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_BoneSocket[i]->m_Name == Name)
			return m_BoneSocket[i];
	}

	return nullptr;
}

void CSkeleton::AddBone(Bone* pBone)
{
	m_vecBones.push_back(pBone);
}

bool CSkeleton::SaveSkeleton(const char* FileName,
	const std::string& PathName)
{
	const PathInfo* pPath = CPathManager::GetInst()->FindPath(PathName);

	char	strFullPath[MAX_PATH] = {};

	if (pPath)
		strcpy_s(strFullPath, pPath->PathMultibyte);

	strcat_s(strFullPath, FileName);

	return SaveSkeletonFullPath(strFullPath);
}

bool CSkeleton::SaveSkeletonFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, FullPath, "wb");

	if (!pFile)
		return false;

	size_t	BoneCount = m_vecBones.size();

	fwrite(&BoneCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < BoneCount; ++i)
	{
		size_t	NameCount = m_vecBones[i]->Name.size();

		fwrite(&NameCount, sizeof(size_t), 1, pFile);
		fwrite(m_vecBones[i]->Name.c_str(), 1, NameCount, pFile);

		fwrite(&m_vecBones[i]->Depth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->ParentIndex, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->matOffset, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i]->matBone, sizeof(Matrix), 1, pFile);
	}

	fclose(pFile);

	return true;
}

bool CSkeleton::LoadSkeleton(CScene* pScene, const std::string& Name,
	const char* FileName, const std::string& PathName)
{
	const PathInfo* pPath = CPathManager::GetInst()->FindPath(PathName);

	char	strFullPath[MAX_PATH] = {};

	if (pPath)
		strcpy_s(strFullPath, pPath->PathMultibyte);

	strcat_s(strFullPath, FileName);

	return LoadSkeletonFullPath(pScene, Name, strFullPath);
}

bool CSkeleton::LoadSkeletonFullPath(CScene* pScene,
	const std::string& Name, const char* FullPath)
{
	m_Scene = pScene;

	FILE* pFile = nullptr;

	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return false;

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		--m_vecBones[i]->RefCount;

		if (m_vecBones[i]->RefCount == 0)
		{
			SAFE_DELETE(m_vecBones[i]);
		}
	}

	m_vecBones.clear();

	size_t	BoneCount = 0;

	fread(&BoneCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < BoneCount; ++i)
	{
		Bone* pBone = new Bone;

		m_vecBones.push_back(pBone);

		size_t	iNameCount = 0;

		char	BoneName[256] = {};
		fread(&iNameCount, sizeof(size_t), 1, pFile);
		fread(BoneName, 1, iNameCount, pFile);
		pBone->Name = BoneName;

		fread(&pBone->Depth, sizeof(int), 1, pFile);
		fread(&pBone->ParentIndex, sizeof(int), 1, pFile);
		fread(&pBone->matOffset, sizeof(Matrix), 1, pFile);
		fread(&pBone->matBone, sizeof(Matrix), 1, pFile);
	}

	fclose(pFile);

	return true;
}

void CSkeleton::SetShader()
{
	if (!m_pOffsetMatrixBuffer)
	{
		m_pOffsetMatrixBuffer = new CStructuredBuffer;

		std::vector<Matrix>	vecOffset;

		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i]->matOffset);
		}

		m_pOffsetMatrixBuffer->Init("OffsetMatrixBuffer", (unsigned int)sizeof(Matrix),
			(unsigned int)vecOffset.size(), 16, true, (int)EShaderBufferType::Compute);

		m_pOffsetMatrixBuffer->UpdateBuffer(&vecOffset[0], (int)vecOffset.size());
	}

	m_pOffsetMatrixBuffer->SetShader();
}

void CSkeleton::ResetShader()
{
	m_pOffsetMatrixBuffer->ResetShader();
}

void CSkeleton::AddSocket(const std::string& BoneName, const std::string& SocketName,
	const Vector3& Offset, const Vector3& OffsetRot)
{
	if (!CheckBone(BoneName))
		return;

	CSkeletonSocket* Socket = new CSkeletonSocket;

	Socket->m_BoneName = BoneName;
	Socket->m_Name = SocketName;
	Socket->m_Offset = Offset;
	Socket->m_OffsetRotation = OffsetRot;
	Socket->m_BoneIndex = GetBoneIndex(BoneName);

	m_BoneSocket.push_back(Socket);
}

void CSkeleton::Update(float fTime, const std::vector<OutputBoneInfo>& vecBoneInfo,
	CAnimationMeshComponent* Owner)
{
	size_t	Size = m_BoneSocket.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_BoneSocket[i]->Update(vecBoneInfo[m_BoneSocket[i]->m_BoneIndex], Owner);
	}
}

CSkeleton* CSkeleton::Clone()
{
	return new CSkeleton(*this);
}

