#pragma once

#include "../EngineInfo.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Sound/Sound.h"
#include "../Resource/Font/Font.h"
#include "../Resource/Font/FontCollection.h"
#include "../Resource/Particle/Particle.h"	

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	class CScene* m_Owner;

private:
	std::unordered_map<std::string, CSharedPtr<CMesh>>		m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CShader>>	m_mapShader;
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CMaterial>>	m_mapMaterial;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapAnimationSequence2D;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>>	m_mapAnimationSequence;
	std::unordered_map<std::string, CSharedPtr<CSkeleton>>	m_mapSkeleton;
	std::unordered_map<std::string, CSharedPtr<CSound>>	m_mapSound;
	std::unordered_map<std::string, CSharedPtr<CFont>>	m_mapFont;
	std::unordered_map<std::string, CSharedPtr<CFontCollection>>	m_mapFontCollection;
	std::unordered_map<std::string, CSharedPtr<CParticle>>	m_mapParticle;

public:
	bool Init();


public:	// ===================== Mesh =========================
	bool CreateMesh(MeshType Type, const std::string& Name,
		void* VtxData, int Size, int Count,
		D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
		void* IdxData = nullptr, int IdxSize = 0, int IdxCount = 0,
		D3D11_USAGE IdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN);
	bool LoadMesh(MeshType Type, const std::string& Name,
		const TCHAR* FileName, const std::string& PathName = MESH_PATH);
	bool LoadMeshFullPath(MeshType Type, const std::string& Name,
		const TCHAR* FullPath);
	bool LoadMeshMultibyte(MeshType Type, const std::string& Name,
		const char* FileName, const std::string& PathName = MESH_PATH);
	bool LoadMeshMultibyteFullPath(MeshType Type, const std::string& Name,
		const char* FullPath);

	class CMesh* FindMesh(const std::string& Name);



public:	// ===================== Shader =========================
	class CShader* FindShader(const std::string& Name);



public:	// ===================== Texture =========================
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);

	bool CreateTarget(const std::string& Name, unsigned int Width,
		unsigned int Height, DXGI_FORMAT PixelFormat,
		DXGI_FORMAT DepthFormat = DXGI_FORMAT_UNKNOWN);
	void RenderTexture();


	class CTexture* FindTexture(const std::string& Name);




public:	// ===================== Material =========================
	CMaterial* FindMaterial(const std::string& Name);

	template <typename T>
	bool CreateMaterial(const std::string& Name)
	{
		if (FindMaterial(Name))
			return true;

		if (!CResourceManager::GetInst()->CreateMaterial<T>(Name))
			return false;

		m_mapMaterial.insert(std::make_pair(Name, CResourceManager::GetInst()->FindMaterial(Name)));

		return true;
	}



public:	// ===================== Animation =========================
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& Name, class CTexture* Texture);
	bool CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const TCHAR* FullPath);
	bool CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFullPath);
	bool AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& End);
	bool AddAnimationSequence2DFrame(const std::string& Name, float StartX, float StartY, float EndX,
		float EndY);
	bool AddAnimationSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End);
	bool AddAnimationSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX,
		float EndY);
	bool SaveSequence2D(const std::string& Name, const char* FullPath);
	bool LoadSequence2D(const std::string& Name, const char* FullPath);
	bool SaveSequence2D(const std::string& Name, const char* FileName, const std::string& PathName);
	bool LoadSequence2D(const std::string& Name, const char* FileName, const std::string& PathName);

	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);



	// Animation3D
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


	bool LoadSkeleton(const std::string& Name, 
		const TCHAR* FileName, const std::string& PathName = ANIMATION_PATH);
	bool LoadSkeletonFullPath(const std::string& Name, 
		const TCHAR* FullPath);
	bool LoadSkeletonMultibyte(const std::string& Name, 
		const char* FileName, const std::string& PathName = ANIMATION_PATH);
	bool LoadSkeletonMultibyteFullPath(const std::string& Name, 
		const char* FullPath);
	void AddSocket(const std::string& SkeletonName,
		const std::string& BoneName, const std::string& SocketName,
		const Vector3& Offset, const Vector3& OffsetRot);

	bool SetMeshSkeleton(const std::string& MeshName,
		const std::string& SkeletonName);

	CSkeleton* FindSkeleton(const std::string& Name);


public:	// ============================ Sound ================================
	bool CreateSoundChannel(const std::string& Name);
	bool LoadSound(const std::string& GroupName, const std::string& Name,
		bool Loop, const char* FileName, const std::string& PathName = SOUND_PATH);
	bool SetVolume(int Volume);
	bool SetVolume(const std::string& GroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);


	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
	class CSound* FindSound(const std::string& Name);



public:	// ============================ Font ================================
	bool CreateFontCollection(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
	bool LoadFont(const std::string& Name, const TCHAR* FontName,
		int Weight, float FontSize, const TCHAR* LocalName, int Stretch = 5);

	const TCHAR* GetFontFaceName(const std::string& CollectionName);
	const char* GetFontFaceNameMultibyte(const std::string& CollectionName);

	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(const Vector4& Color);
	bool CreateFontColor(unsigned int Color);

	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& Color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int Color);

	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(const Vector4& Color);

	CFont* FindFont(const std::string& Name);
	CFontCollection* FindFontCollection(const std::string& Name);

	public:
		bool CreateParticle(const std::string& Name);
		CParticle* FindParticle(const std::string& Name);
};

