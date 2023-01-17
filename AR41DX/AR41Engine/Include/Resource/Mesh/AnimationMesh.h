#pragma once
#include "Mesh.h"
class CAnimationMesh :
    public CMesh
{
	friend class CMeshManager;

protected:
	CAnimationMesh();
	virtual ~CAnimationMesh();

private:
	CSharedPtr<class CSkeleton>	m_Skeleton;
	class CStructuredBuffer* m_BoneBuffer;
	int			m_InstancingCount;

public:
	int GetBoneCount()	const;

	class CStructuredBuffer* GetBoneBuffer()	const
	{
		return m_BoneBuffer;
	}

	class CSkeleton* GetSkeleton()	const
	{
		return m_Skeleton;
	}

	void SetBoneShader();
	void ResetBoneShader();

	void SetSkeleton(class CSkeleton* Skeleton);
	void SetSkeleton(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = MESH_PATH);
	void SetSkeletonFullPath(const std::string& Name,
		const TCHAR* FullPath);
	void SetSkeletonMultibyte(const std::string& Name, const char* FileName,
		const std::string& PathName = MESH_PATH);
	void SetSkeletonMultibyteFullPath(const std::string& Name,
		const char* FullPath);
	void ResizeBoneBuffer(int Count);

public:
	virtual bool LoadMesh(const std::string& Name,
		const TCHAR* FileName, const std::string& PathName = MESH_PATH);
	virtual bool LoadMeshFullPath(const std::string& Name,
		const TCHAR* FullPath);
	virtual bool LoadMeshMultibyte(const std::string& Name,
		const char* FileName, const std::string& PathName = MESH_PATH);
	virtual bool LoadMeshMultibyteFullPath(const std::string& Name,
		const char* FullPath);

protected:
	virtual bool ConvertFBX(class CFBXLoader* Loader, const char* FullPath);
	virtual bool SaveMesh(FILE* File);
	virtual bool LoadMesh(FILE* File);
};

