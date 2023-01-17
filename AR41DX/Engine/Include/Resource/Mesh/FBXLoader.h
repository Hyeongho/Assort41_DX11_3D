#pragma once

#include "../../EngineInfo.h"
#include "fbxsdk.h"

#ifdef _DEBUG

#pragma comment(lib, "../Bin/libfbxsdk-md_Debug.lib")

#else

#pragma comment(lib, "../Bin/libfbxsdk-md.lib")

#endif // _DEBUG

struct FbxBone
{
	std::string     Name;
	int             Depth;
	int             ParentIndex;
	FbxAMatrix      matOffset;
	FbxAMatrix      matBone;
};

struct FbxKeyFrame
{
	FbxAMatrix	matTransform;
	double		Time;
};

struct FbxBoneKeyFrame
{
	int			BoneIndex;
	std::vector<FbxKeyFrame>	vecKeyFrame;
};

struct FbxAnimationClip
{
	std::string		Name;
	FbxTime			StartTime;
	FbxTime			EndTime;
	FbxLongLong		TimeLength;
	FbxTime::EMode	TimeMode;
	std::vector<FbxBoneKeyFrame>	vecBoneKeyFrame;
};


struct FbxWeight
{
	int		Index;
	double	Weight;
};

struct FbxMaterial
{
	Vector4	BaseColor;
	Vector4	AmbientColor;
	Vector4	SpecularColor;
	Vector4	EmissiveColor;
	float	SpecularPower;
	float	TransparencyFactor;
	float	Shininess;
	std::string	DiffuseTexture;
	std::string	BumpTexture;
	std::string	SpecularTexture;
};

struct FbxMeshContainer
{
	std::vector<Vector3>	vecPos;
	std::vector<Vector3>	vecNormal;
	std::vector<Vector2>	vecUV;
	std::vector<Vector3>	vecTangent;
	std::vector<Vector3>	vecBinormal;
	std::vector<Vector4>	vecBlendWeight;
	std::vector<Vector4>	vecBlendIndex;
	std::vector<std::vector<unsigned int>>	vecIndex;
	std::unordered_map<int, std::vector<FbxWeight>>	mapWeights;
	bool	Bump;
	bool	Animation;

	FbxMeshContainer()	:
		Bump(false),
		Animation(false)
	{
	}
};

class CFBXLoader
{
	friend class CMesh;
	friend class CStaticMesh;
	friend class CAnimationMesh;
	friend class CAnimationSequence;

private:
	CFBXLoader();
	~CFBXLoader();

private:
	FbxManager* m_Manager;
	FbxScene* m_Scene;

private:
	std::vector<FbxMeshContainer*>	m_vecMeshContainer;
	std::vector<std::vector<FbxMaterial*>>	m_vecMaterial;
	std::vector<FbxBone*>			m_vecBone;
	FbxArray<FbxString*>			m_NameArray;
	std::vector<FbxAnimationClip*>	m_vecClip;
	bool							m_Mixamo;

public:
	const std::vector<FbxMeshContainer*>* GetContainer()	const
	{
		return &m_vecMeshContainer;
	}

	const std::vector<std::vector<FbxMaterial*>>* GetMaterials()	const
	{
		return &m_vecMaterial;
	}

	const std::vector<FbxBone*>* GetBones()	const
	{
		return &m_vecBone;
	}

	const std::vector<FbxAnimationClip*>* GetClips()	const
	{
		return &m_vecClip;
	}

public:
	bool LoadFBX(const char* FullPath, bool Static = true);

private:
	void Triangulate(FbxNode* Node);
	void LoadMaterial(FbxSurfaceMaterial* Mtrl);
	Vector4 GetMaterialColor(FbxSurfaceMaterial* Mtrl,
		const char* PropertyName, const char* PropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial* Mtrl,
		const char* PropertyName);
	std::string GetMaterialTexture(FbxSurfaceMaterial* Mtrl,
		const char* PropertyName);
	void LoadMesh(FbxNode* Node, bool Static);
	void LoadMesh(FbxMesh* Mesh, bool Static);

	void LoadNormal(FbxMesh* Mesh, FbxMeshContainer* Container,
		int VtxID,
		int ControlIndex);
	void LoadUV(FbxMesh* Mesh, FbxMeshContainer* pContainer,
		int UVID, int ControlIndex);
	void LoadTangent(FbxMesh* Mesh,
		FbxMeshContainer* Container, int VtxID, int ControlIndex);
	void LoadBinormal(FbxMesh* Mesh, FbxMeshContainer* Container,
		int VtxID, int ControlIndex);

private:
	void LoadAnimationClip();
	void LoadBone(FbxNode* Node);
	void LoadBoneRecursive(FbxNode* Node, int Depth, int Index, int Parent);
	void LoadAnimation(FbxMesh* Mesh, FbxMeshContainer* Container);
	FbxAMatrix GetTransform(FbxNode* Node);
	int FindBoneFromName(const std::string& Name);
	void LoadWeightAndIndex(FbxCluster* Cluster,
		int BoneIndex, FbxMeshContainer* Container);
	void LoadOffsetMatrix(FbxCluster* Cluster,
		const FbxAMatrix& matTransform, int BoneIndex,
		FbxMeshContainer* Container);
	void LoadTimeTransform(FbxNode* Node,
		FbxCluster* Cluster, const FbxAMatrix& matTransform,
		int BoneIndex);
	void ChangeWeightAndIndices(FbxMeshContainer* Container);
};

