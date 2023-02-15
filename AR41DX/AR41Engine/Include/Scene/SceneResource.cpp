
#include "SceneResource.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	{
		auto	iter = m_mapMesh.begin();
		auto	iterEnd = m_mapMesh.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapMesh.erase(iter);
			iterEnd = m_mapMesh.end();

			CResourceManager::GetInst()->ReleaseMesh(Name);
		}
	}

	{
		auto	iter = m_mapShader.begin();
		auto	iterEnd = m_mapShader.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapShader.erase(iter);
			iterEnd = m_mapShader.end();

			CResourceManager::GetInst()->ReleaseShader(Name);
		}
	}

	{
		auto	iter = m_mapTexture.begin();
		auto	iterEnd = m_mapTexture.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapTexture.erase(iter);
			iterEnd = m_mapTexture.end();

			CResourceManager::GetInst()->ReleaseTexture(Name);
		}
	}

	{
		auto	iter = m_mapAnimationSequence2D.begin();
		auto	iterEnd = m_mapAnimationSequence2D.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapAnimationSequence2D.erase(iter);
			iterEnd = m_mapAnimationSequence2D.end();

			CResourceManager::GetInst()->ReleaseAnimationSequence2D(Name);
		}
	}

	{
		auto	iter = m_mapAnimationSequence.begin();
		auto	iterEnd = m_mapAnimationSequence.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapAnimationSequence.erase(iter);
			iterEnd = m_mapAnimationSequence.end();

			CResourceManager::GetInst()->ReleaseAnimationSequence(Name);
		}
	}

	{
		auto	iter = m_mapSkeleton.begin();
		auto	iterEnd = m_mapSkeleton.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapSkeleton.erase(iter);
			iterEnd = m_mapSkeleton.end();

			CResourceManager::GetInst()->ReleaseSkeleton(Name);
		}
	}

	{
		auto	iter = m_mapSound.begin();
		auto	iterEnd = m_mapSound.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapSound.erase(iter);
			iterEnd = m_mapSound.end();

			CResourceManager::GetInst()->ReleaseSound(Name);
		}
	}


	{
		auto	iter = m_mapFont.begin();
		auto	iterEnd = m_mapFont.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapFont.erase(iter);
			iterEnd = m_mapFont.end();

			CResourceManager::GetInst()->ReleaseFont(Name);
		}
	}


	{
		auto	iter = m_mapFontCollection.begin();
		auto	iterEnd = m_mapFontCollection.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapFontCollection.erase(iter);
			iterEnd = m_mapFontCollection.end();

			CResourceManager::GetInst()->ReleaseFontCollection(Name);
		}
	}

	{
		auto	iter = m_mapParticle.begin();
		auto	iterEnd = m_mapParticle.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapParticle.erase(iter);
			iterEnd = m_mapParticle.end();

			CResourceManager::GetInst()->ReleaseParticle(Name);
		}
	}
}

bool CSceneResource::Init()
{
	return true;
}

bool CSceneResource::CreateMesh(MeshType Type, const std::string& Name, void* VtxData, 
	int Size, int Count, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
	void* IdxData, int IdxSize, int IdxCount, D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
	if (FindMesh(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateMesh(m_Owner, Type, Name, VtxData, Size, Count,
		VtxUsage, Primitive, IdxData, IdxSize,
		IdxCount, IdxUsage, Fmt))
		return false;

	m_mapMesh.insert(std::make_pair(Name, CResourceManager::GetInst()->FindMesh(Name)));

	return true;
}

bool CSceneResource::LoadMesh(MeshType Type, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (FindMesh(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadMesh(m_Owner, Type, Name,
		FileName, PathName))
		return false;

	m_mapMesh.insert(std::make_pair(Name, CResourceManager::GetInst()->FindMesh(Name)));

	return true;
}

bool CSceneResource::LoadMeshFullPath(MeshType Type,
	const std::string& Name, const TCHAR* FullPath)
{
	if (FindMesh(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadMeshFullPath(m_Owner, Type, Name,
		FullPath))
		return false;

	m_mapMesh.insert(std::make_pair(Name, CResourceManager::GetInst()->FindMesh(Name)));

	return true;
}

bool CSceneResource::LoadMeshMultibyte(MeshType Type, 
	const std::string& Name, const char* FileName, const std::string& PathName)
{
	if (FindMesh(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadMeshMultibyte(m_Owner, Type, Name,
		FileName, PathName))
		return false;

	m_mapMesh.insert(std::make_pair(Name, CResourceManager::GetInst()->FindMesh(Name)));

	return true;
}

bool CSceneResource::LoadMeshMultibyteFullPath(MeshType Type,
	const std::string& Name, const char* FullPath)
{
	if (FindMesh(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadMeshMultibyteFullPath(m_Owner, Type, Name,
		FullPath))
		return false;

	m_mapMesh.insert(std::make_pair(Name, CResourceManager::GetInst()->FindMesh(Name)));

	return true;
}

CMesh* CSceneResource::FindMesh(const std::string& Name)
{
	auto	iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
	{
		CMesh* Mesh = CResourceManager::GetInst()->FindMesh(Name);

		if (!Mesh)
			return nullptr;

		m_mapMesh.insert(std::make_pair(Name, Mesh));

		return Mesh;
	}

	return iter->second;
}

CShader* CSceneResource::FindShader(const std::string& Name)
{
	auto	iter = m_mapShader.find(Name);

	if (iter == m_mapShader.end())
	{
		CShader* Shader = CResourceManager::GetInst()->FindShader(Name);

		if (!Shader)
			return nullptr;

		m_mapShader.insert(std::make_pair(Name, Shader));

		return Shader;
	}

	return iter->second;
}

bool CSceneResource::LoadTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

bool CSceneResource::LoadTexture(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

bool CSceneResource::LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureArray(Name, vecFileName, PathName))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

bool CSceneResource::LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(Name, vecFullPath))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

bool CSceneResource::CreateTarget(const std::string& Name,
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat,
	DXGI_FORMAT DepthFormat)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateTarget(Name, Width, Height,
		PixelFormat, DepthFormat))
		return false;

	m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));

	return true;
}

void CSceneResource::RenderTexture()
{
	CResourceManager::GetInst()->RenderTexture();
}

CTexture* CSceneResource::FindTexture(const std::string& Name)
{
	auto	iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
	{
		CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

		if (!Texture)
			return nullptr;

		m_mapTexture.insert(std::make_pair(Name, Texture));

		return Texture;
	}

	return iter->second;
}

CMaterial* CSceneResource::FindMaterial(const std::string& Name)
{
	auto	iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
	{
		CMaterial* Material = CResourceManager::GetInst()->FindMaterial(Name);

		if (!Material)
			return nullptr;

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return Material;
	}

	return iter->second;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& Name, 
	const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	if (FindAnimationSequence2D(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(Name, TextureName,
		FileName, PathName))
		return false;

	m_mapAnimationSequence2D.insert(std::make_pair(Name, CResourceManager::GetInst()->FindAnimationSequence2D(Name)));

	return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& Name, 
	CTexture* Texture)
{
	if (FindAnimationSequence2D(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(Name, Texture))
		return false;

	m_mapAnimationSequence2D.insert(std::make_pair(Name, CResourceManager::GetInst()->FindAnimationSequence2D(Name)));

	return true;
}

bool CSceneResource::CreateAnimationSequence2DFullPath(
	const std::string& Name, const std::string& TextureName,
	const TCHAR* FullPath)
{
	if (FindAnimationSequence2D(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence2DFullPath(Name, TextureName,
		FullPath))
		return false;

	m_mapAnimationSequence2D.insert(std::make_pair(Name, CResourceManager::GetInst()->FindAnimationSequence2D(Name)));

	return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& Name, 
	const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFileName, 
	const std::string& PathName)
{
	if (FindAnimationSequence2D(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(Name, TextureName,
		vecFileName, PathName))
		return false;

	m_mapAnimationSequence2D.insert(std::make_pair(Name, CResourceManager::GetInst()->FindAnimationSequence2D(Name)));

	return true;
}

bool CSceneResource::CreateAnimationSequence2DFullPath(
	const std::string& Name, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (FindAnimationSequence2D(Name))
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence2DFullPath(Name, TextureName,
		vecFullPath))
		return false;

	m_mapAnimationSequence2D.insert(std::make_pair(Name, CResourceManager::GetInst()->FindAnimationSequence2D(Name)));

	return true;
}

bool CSceneResource::AddAnimationSequence2DFrame(const std::string& Name,
	const Vector2& Start, const Vector2& End)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrame(Name, Start, End);
}

bool CSceneResource::AddAnimationSequence2DFrame(const std::string& Name,
	float StartX, float StartY, float EndX, float EndY)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrame(Name, StartX, StartY, EndX, EndY);
}

bool CSceneResource::AddAnimationSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrameAll(Name, Count, Start, End);
}

bool CSceneResource::AddAnimationSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX, float EndY)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrameAll(Name, Count, StartX, StartY, EndX, EndY);
}

bool CSceneResource::SaveSequence2D(const std::string& Name, const char* FullPath)
{
	return CResourceManager::GetInst()->SaveSequence2D(Name, FullPath);
}

bool CSceneResource::LoadSequence2D(const std::string& Name, const char* FullPath)
{
	return CResourceManager::GetInst()->LoadSequence2D(Name, FullPath);
}

bool CSceneResource::SaveSequence2D(const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	return CResourceManager::GetInst()->SaveSequence2D(Name, FileName, PathName);
}

bool CSceneResource::LoadSequence2D(const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	return CResourceManager::GetInst()->LoadSequence2D(Name, FileName, PathName);
}

CAnimationSequence2D* CSceneResource::FindAnimationSequence2D(const std::string& Name)
{
	auto	iter = m_mapAnimationSequence2D.find(Name);

	if (iter == m_mapAnimationSequence2D.end())
	{
		CAnimationSequence2D* Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(Name);

		if (!Sequence)
			return nullptr;

		m_mapAnimationSequence2D.insert(std::make_pair(Name, Sequence));

		return Sequence;
	}

	return iter->second;
}
bool CSceneResource::LoadAnimationSequence(const std::string& Name,
	bool Loop, const FbxAnimationClip* Clip)
{
	if (FindAnimationSequence(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequence(Name,
		Loop, Clip))
		return false;

	m_mapAnimationSequence.insert(std::make_pair(Name, 
		CResourceManager::GetInst()->FindAnimationSequence(Name)));

	return true;
}
bool CSceneResource::LoadAnimationSequence(const std::string& Name,
	bool Loop, int StartFrame, int EndFrame, float PlayTime, 
	const std::vector<BoneKeyFrame*>& vecFrame)
{
	if (FindAnimationSequence(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequence(Name,
		Loop, StartFrame, EndFrame, PlayTime, vecFrame))
		return false;

	m_mapAnimationSequence.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindAnimationSequence(Name)));

	return true;
}
bool CSceneResource::LoadAnimationSequence(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (FindAnimationSequence(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequence(Name,
		FileName, PathName))
		return false;

	m_mapAnimationSequence.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindAnimationSequence(Name)));

	return true;
}
bool CSceneResource::LoadAnimationSequenceFullPath(
	const std::string& Name, const TCHAR* FullPath)
{
	if (FindAnimationSequence(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequenceFullPath(Name,
		FullPath))
		return false;

	m_mapAnimationSequence.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindAnimationSequence(Name)));

	return true;
}
bool CSceneResource::LoadAnimationSequenceMultibyte(
	const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	if (FindAnimationSequence(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequenceMultibyte(Name,
		FileName, PathName))
		return false;

	m_mapAnimationSequence.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindAnimationSequence(Name)));

	return true;
}
bool CSceneResource::LoadAnimationSequenceMultibyteFullPath(
	const std::string& Name, const char* FullPath)
{
	if (FindAnimationSequence(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadAnimationSequenceMultibyteFullPath(Name,
		FullPath))
		return false;

	m_mapAnimationSequence.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindAnimationSequence(Name)));

	return true;
}
CAnimationSequence* CSceneResource::FindAnimationSequence(
	const std::string& Name)
{
	auto	iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
	{
		CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(Name);

		if (!Sequence)
			return nullptr;

		m_mapAnimationSequence.insert(std::make_pair(Name, Sequence));

		return Sequence;
	}

	return iter->second;
}
bool CSceneResource::LoadSkeleton(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	if (FindSkeleton(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadSkeleton(m_Owner, Name,
		FileName, PathName))
		return false;

	m_mapSkeleton.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindSkeleton(Name)));

	return true;
}
bool CSceneResource::LoadSkeletonFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	if (FindSkeleton(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadSkeletonFullPath(m_Owner, Name,
		FullPath))
		return false;

	m_mapSkeleton.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindSkeleton(Name)));

	return true;
}
bool CSceneResource::LoadSkeletonMultibyte(const std::string& Name,
	const char* FileName, const std::string& PathName)
{
	if (FindSkeleton(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadSkeletonMultibyte(m_Owner, Name,
		FileName, PathName))
		return false;

	m_mapSkeleton.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindSkeleton(Name)));

	return true;
}
bool CSceneResource::LoadSkeletonMultibyteFullPath(
	const std::string& Name, const char* FullPath)
{
	if (FindSkeleton(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadSkeletonMultibyteFullPath(m_Owner, Name,
		FullPath))
		return false;

	m_mapSkeleton.insert(std::make_pair(Name,
		CResourceManager::GetInst()->FindSkeleton(Name)));

	return true;
}
void CSceneResource::AddSocket(const std::string& SkeletonName, const std::string& BoneName, const std::string& SocketName, const Vector3& Offset, const Vector3& OffsetRot)
{
	CResourceManager::GetInst()->AddSocket(SkeletonName, BoneName, SocketName, Offset, OffsetRot);
}

bool CSceneResource::SetMeshSkeleton(const std::string& MeshName, 
	const std::string& SkeletonName)
{
	return CResourceManager::GetInst()->SetMeshSkeleton(MeshName,
		SkeletonName);
}
CSkeleton* CSceneResource::FindSkeleton(const std::string& Name)
{
	auto	iter = m_mapSkeleton.find(Name);

	if (iter == m_mapSkeleton.end())
	{
		CSkeleton* Skeleton = CResourceManager::GetInst()->FindSkeleton(Name);

		if (!Skeleton)
			return nullptr;

		m_mapSkeleton.insert(std::make_pair(Name, Skeleton));

		return Skeleton;
	}

	return iter->second;
}
bool CSceneResource::CreateSoundChannel(const std::string& Name)
{
	return CResourceManager::GetInst()->CreateSoundChannel(Name);
}

bool CSceneResource::LoadSound(const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName)
{
	if (FindSound(Name))
		return false;

	if (!CResourceManager::GetInst()->LoadSound(GroupName, Name, Loop, FileName, PathName))
		return false;

	CSound* Sound = CResourceManager::GetInst()->FindSound(Name);

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}

bool CSceneResource::SetVolume(float Volume)
{
	return CResourceManager::GetInst()->SetVolume(Volume);
}

bool CSceneResource::SetVolume(const std::string& GroupName, float Volume)
{
	return CResourceManager::GetInst()->SetVolume(GroupName, Volume);
}

bool CSceneResource::SoundPlay(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPlay(Name);
}

bool CSceneResource::SoundStop(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundStop(Name);
}

bool CSceneResource::SoundPause(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPause(Name);
}

bool CSceneResource::SoundResume(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundResume(Name);
}

FMOD::ChannelGroup* CSceneResource::FindChannelGroup(const std::string& Name)
{
	return CResourceManager::GetInst()->FindChannelGroup(Name);
}

CSound* CSceneResource::FindSound(const std::string& Name)
{
	auto	iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
	{
		CSound* Sound = CResourceManager::GetInst()->FindSound(Name);

		if (!Sound)
			return nullptr;

		m_mapSound.insert(std::make_pair(Name, Sound));

		return Sound;
	}

	return iter->second;
}

bool CSceneResource::CreateFontCollection(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (FindFontCollection(Name))
		return false;

	if (!CResourceManager::GetInst()->CreateFontCollection(Name, FileName, PathName))
		return false;

	CFontCollection* Font = CResourceManager::GetInst()->FindFontCollection(Name);

	m_mapFontCollection.insert(std::make_pair(Name, Font));

	return true;
}

bool CSceneResource::LoadFont(const std::string& Name, const TCHAR* FontName,
	int Weight, float FontSize, const TCHAR* LocalName, int Stretch)
{
	if (FindFont(Name))
		return false;

	if (!CResourceManager::GetInst()->LoadFont(Name, FontName, Weight,
		FontSize, LocalName, Stretch))
		return false;

	CFont* Font = CResourceManager::GetInst()->FindFont(Name);

	m_mapFont.insert(std::make_pair(Name, Font));

	return true;
}

const TCHAR* CSceneResource::GetFontFaceName(const std::string& CollectionName)
{
	return CResourceManager::GetInst()->GetFontFaceName(CollectionName);
}

const char* CSceneResource::GetFontFaceNameMultibyte(const std::string& CollectionName)
{
	return CResourceManager::GetInst()->GetFontFaceNameMultibyte(CollectionName);
}

bool CSceneResource::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return CResourceManager::GetInst()->CreateFontColor(r, g, b, a);
}

bool CSceneResource::CreateFontColor(float r, float g, float b, float a)
{
	return CResourceManager::GetInst()->CreateFontColor(r, g, b, a);
}

bool CSceneResource::CreateFontColor(const Vector4& Color)
{
	return CResourceManager::GetInst()->CreateFontColor(Color);
}

bool CSceneResource::CreateFontColor(unsigned int Color)
{
	return CResourceManager::GetInst()->CreateFontColor(Color);
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return CResourceManager::GetInst()->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(float r, float g, float b, float a)
{
	return CResourceManager::GetInst()->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(const Vector4& Color)
{
	return CResourceManager::GetInst()->FindFontColor(Color);
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(unsigned int Color)
{
	return CResourceManager::GetInst()->FindFontColor(Color);
}

unsigned int CSceneResource::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return CResourceManager::GetInst()->CreateFontColorKey(r, g, b, a);
}

unsigned int CSceneResource::CreateFontColorKey(float r, float g, float b, float a)
{
	return CResourceManager::GetInst()->CreateFontColorKey(r, g, b, a);
}

unsigned int CSceneResource::CreateFontColorKey(const Vector4& Color)
{
	return CResourceManager::GetInst()->CreateFontColorKey(Color);
}

CFont* CSceneResource::FindFont(const std::string& Name)
{
	auto	iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
	{
		CFont* Font = CResourceManager::GetInst()->FindFont(Name);

		if (!Font)
			return nullptr;

		m_mapFont.insert(std::make_pair(Name, Font));

		return Font;
	}

	return iter->second;
}

CFontCollection* CSceneResource::FindFontCollection(const std::string& Name)
{
	auto	iter = m_mapFontCollection.find(Name);

	if (iter == m_mapFontCollection.end())
	{
		CFontCollection* Font = CResourceManager::GetInst()->FindFontCollection(Name);

		if (!Font)
			return nullptr;

		m_mapFontCollection.insert(std::make_pair(Name, Font));

		return Font;
	}

	return iter->second;
}

bool CSceneResource::CreateParticle(const std::string& Name)
{
	if (FindParticle(Name))
	{
		return false;
	}

	if (!CResourceManager::GetInst()->CreateParticle(Name))
	{
		return false;
	}

	CParticle* Particle = CResourceManager::GetInst()->FindParticle(Name);

	m_mapParticle.insert(std::make_pair(Name, Particle));

	return true;
}

CParticle* CSceneResource::FindParticle(const std::string& Name)
{
	auto iter = m_mapParticle.find(Name);

	if (iter == m_mapParticle.end())
	{
		CParticle* Particle = CResourceManager::GetInst()->FindParticle(Name);

		if (!Particle)
		{
			return nullptr;
		}

		m_mapParticle.insert(std::make_pair(Name, Particle));

		return Particle;
	}

	return iter->second;
}

