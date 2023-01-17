#pragma once

#include "../../Ref.h"

struct Bone
{
    std::string     Name;
    int             Depth;
    int             ParentIndex;
    Matrix          matOffset;
    Matrix          matBone;
    int             RefCount;

    Bone()  :
        RefCount(1)
    {
    }
};

class CSkeleton :
    public CRef
{
    friend class CAnimationManager;
    friend class CAnimationMesh;
    friend class CAnimationSequence;
    friend class CAnimationSequenceInstance;

private:
    CSkeleton();
    CSkeleton(const CSkeleton& Skeleton);
    ~CSkeleton();

private:
	std::vector<Bone*>				m_vecBones;
	class CStructuredBuffer* m_pOffsetMatrixBuffer;
	class CScene* m_Scene;
	std::vector<class CSkeletonSocket*>	m_BoneSocket;

public:
	size_t GetBoneCount()	const;
	Bone* GetBone(int Index = 0)	const;
	Bone* GetBone(const std::string& Name)	const;
	int GetBoneIndex(const std::string& Name)	const;
	bool CheckBone(const std::string& Name)	const;
	const Matrix& GetBoneMatrix(const std::string& Name)	const;
	const Matrix& GetBoneMatrix(int Index)	const;
	class CSkeletonSocket* GetSocket(const std::string& Name);

public:
	void AddBone(Bone* pBone);
	bool SaveSkeleton(const char* FileName, const std::string& strPathName = MESH_PATH);
	bool SaveSkeletonFullPath(const char* FullPath);
	bool LoadSkeleton(class CScene* pScene, const std::string& strName, const char* FileName, const std::string& strPathName = MESH_PATH);
	bool LoadSkeletonFullPath(class CScene* pScene, const std::string& strName, const char* FullPath);
	void SetShader();
	void ResetShader();
	void AddSocket(const std::string& BoneName, const std::string& SocketName,
		const Vector3& Offset, const Vector3& OffsetRot);
	void Update(float DeltaTime, const std::vector<OutputBoneInfo>& vecBoneInfo,
		class CAnimationMeshComponent* Owner);
	CSkeleton* Clone();
};

