#include "ResourceManager.h"
#include "Mesh/MeshManager.h"
#include "Texture/TextureManager.h"
#include "../UI/UIWidget.h"
#include "../UI/UIProgressBar.h"
#include "Shader/UIProgressBarConstantBuffer.h"
#include "Mesh/AnimationMesh.h"
#include "Animation/Skeleton.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(CUIProgressBar::m_ProgressBarCBuffer);
	SAFE_DELETE(CUIWidget::m_CBuffer);
	SAFE_DELETE(CUIWidget::m_AnimCBuffer);

	SAFE_DELETE(m_FontManager);
	SAFE_DELETE(m_SoundManager);
	SAFE_DELETE(m_AnimationManager);
	SAFE_DELETE(m_MaterialManager);
	SAFE_DELETE(m_TextureManager);
	SAFE_DELETE(m_ShaderManager);
	SAFE_DELETE(m_MeshManager);
	SAFE_DELETE(m_ParticleManager);
}

bool CResourceManager::Init()
{
	m_ShaderManager = new CShaderManager;

	m_ShaderManager->Init();

	CUIWidget::m_CBuffer = new CUIConstantBuffer;

	CUIWidget::m_CBuffer->Init();

	CUIWidget::m_AnimCBuffer = new CAnimation2DConstantBuffer;

	CUIWidget::m_AnimCBuffer->Init();

	CUIProgressBar::m_ProgressBarCBuffer = new CUIProgressBarConstantBuffer;

	CUIProgressBar::m_ProgressBarCBuffer->Init();

	m_TextureManager = new CTextureManager;

	m_TextureManager->Init();

	m_MaterialManager = new CMaterialManager;

	m_MaterialManager->Init();

	m_MeshManager = new CMeshManager;

	m_MeshManager->Init();

	m_AnimationManager = new CAnimationManager;

	m_AnimationManager->Init();

	m_SoundManager = new CSoundManager;

	m_SoundManager->Init();

	m_FontManager = new CFontManager;

	m_FontManager->Init();

	m_ParticleManager = new CParticleManager;

	m_ParticleManager->Init();

	return true;
}

void CResourceManager::Update()
{
	m_SoundManager->Update();
}

bool CResourceManager::CreateMesh(CScene* Scene, MeshType Type,
	const std::string& Name, void* VtxData, int Size, 
	int Count, D3D11_USAGE VtxUsage, 
	D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IdxData, int IdxSize, 
	int IdxCount, D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
	return m_MeshManager->CreateMesh(Scene, Type, Name, VtxData, Size,
		Count, VtxUsage, Primitive, IdxData, IdxSize, IdxCount, IdxUsage,
		Fmt);
}

bool CResourceManager::LoadMesh(CScene* Scene, MeshType Type, const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	return m_MeshManager->LoadMesh(Scene, Type, Name, FileName, PathName);
}

bool CResourceManager::LoadMeshFullPath(CScene* Scene, MeshType Type, const std::string& Name, const TCHAR* FullPath)
{
	return m_MeshManager->LoadMeshFullPath(Scene, Type, Name, FullPath);
}

bool CResourceManager::LoadMeshMultibyte(CScene* Scene, MeshType Type,
	const std::string& Name, const char* FileName, const std::string& PathName)
{
	return m_MeshManager->LoadMeshMultibyte(Scene, Type, Name, FileName, PathName);
}

bool CResourceManager::LoadMeshMultibyteFullPath(CScene* Scene, 
	MeshType Type, const std::string& Name, const char* FullPath)
{
	return m_MeshManager->LoadMeshMultibyteFullPath(Scene, Type, Name, FullPath);
}

CMesh* CResourceManager::FindMesh(const std::string& Name)
{
	return m_MeshManager->FindMesh(Name);
}

void CResourceManager::ReleaseMesh(const std::string& Name)
{
	m_MeshManager->ReleaseMesh(Name);
}

CColliderConstantBuffer* CResourceManager::GetColliderCBuffer() const
{
	return m_ShaderManager->GetColliderCBuffer();
}

CShader* CResourceManager::FindShader(const std::string& Name)
{
	return m_ShaderManager->FindShader(Name);
}

void CResourceManager::ReleaseShader(const std::string& Name)
{
	m_ShaderManager->ReleaseShader(Name);
}

bool CResourceManager::CreateConstantBuffer(const std::string& Name, int Size,
	int Register, int ShaderBufferType)
{
	return m_ShaderManager->CreateConstantBuffer(Name, Size, Register, ShaderBufferType);
}

CConstantBuffer* CResourceManager::FindConstantBuffer(const std::string& Name)
{
	return m_ShaderManager->FindConstantBuffer(Name);
}

bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	return m_TextureManager->LoadTexture(Name, FileName, PathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	return m_TextureManager->LoadTextureFullPath(Name, FullPath);
}

bool CResourceManager::LoadTexture(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	return m_TextureManager->LoadTexture(Name, vecFileName, PathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	return m_TextureManager->LoadTextureFullPath(Name, vecFullPath);
}

bool CResourceManager::LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	return m_TextureManager->LoadTextureArray(Name, vecFileName, PathName);
}

bool CResourceManager::LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	return m_TextureManager->LoadTextureArrayFullPath(Name, vecFullPath);
}

bool CResourceManager::CreateTarget(const std::string& Name, 
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat,
	DXGI_FORMAT DepthFormat)
{
	return m_TextureManager->CreateTarget(Name, Width, Height,
		PixelFormat, DepthFormat);
}

void CResourceManager::RenderTexture()
{
	m_TextureManager->Render();
}

CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	return m_TextureManager->FindTexture(Name);
}

void CResourceManager::ReleaseTexture(const std::string& Name)
{
	m_TextureManager->ReleaseTexture(Name);
}

CMaterial* CResourceManager::FindMaterial(const std::string& Name)
{
	return m_MaterialManager->FindMaterial(Name);
}

void CResourceManager::ReleaseMaterial(const std::string& Name)
{
	m_MaterialManager->ReleaseMaterial(Name);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	return m_AnimationManager->CreateAnimationSequence2D(Name, TextureName, FileName, PathName);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& Name, CTexture* Texture)
{
	return m_AnimationManager->CreateAnimationSequence2D(Name, Texture);
}

bool CResourceManager::CreateAnimationSequence2DFullPath(
	const std::string& Name, const std::string& TextureName,
	const TCHAR* FullPath)
{
	return m_AnimationManager->CreateAnimationSequence2DFullPath(Name, TextureName, FullPath);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& Name,
	const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	return m_AnimationManager->CreateAnimationSequence2D(Name, TextureName,
		vecFileName, PathName);
}

bool CResourceManager::CreateAnimationSequence2DFullPath(
	const std::string& Name, const std::string& TextureName, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	return m_AnimationManager->CreateAnimationSequence2DFullPath(Name, TextureName,
		vecFullPath);
}

bool CResourceManager::AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& End)
{
	return m_AnimationManager->AddSequence2DFrame(Name, Start, End);
}

bool CResourceManager::AddAnimationSequence2DFrame(const std::string& Name, float StartX, float StartY, float EndX, float EndY)
{
	return m_AnimationManager->AddSequence2DFrame(Name, StartX, StartY, EndX, EndY);
}

bool CResourceManager::AddAnimationSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End)
{
	return m_AnimationManager->AddSequence2DFrameAll(Name, Count, Start, End);
}

bool CResourceManager::AddAnimationSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX, float EndY)
{
	return m_AnimationManager->AddSequence2DFrameAll(Name, Count, StartX, StartY, EndX, EndY);
}

bool CResourceManager::SaveSequence2D(const std::string& Name, const char* FullPath)
{
	return m_AnimationManager->SaveSequence2D(Name, FullPath);
}

bool CResourceManager::LoadSequence2D(const std::string& Name, const char* FullPath)
{
	return m_AnimationManager->LoadSequence2D(Name, FullPath);
}

bool CResourceManager::SaveSequence2D(const std::string& Name, const char* FileName, const std::string& PathName)
{
	return m_AnimationManager->SaveSequence2D(Name, FileName, PathName);
}

bool CResourceManager::LoadSequence2D(const std::string& Name, const char* FileName, const std::string& PathName)
{
	return m_AnimationManager->LoadSequence2D(Name, FileName, PathName);
}

CAnimationSequence2D* CResourceManager::FindAnimationSequence2D(const std::string& Name)
{
	return m_AnimationManager->FindAnimationSequence2D(Name);
}

void CResourceManager::ReleaseAnimationSequence2D(const std::string& Name)
{
	m_AnimationManager->ReleaseAnimationSequence2D(Name);
}

CAnimation2DConstantBuffer* CResourceManager::GetAnim2DConstantBuffer() const
{
	return m_AnimationManager->GetAnim2DConstantBuffer();
}

bool CResourceManager::LoadAnimationSequence(
	const std::string& Name, bool Loop, 
	const FbxAnimationClip* Clip)
{
	return m_AnimationManager->LoadAnimationSequence(Name, Loop, Clip);
}

bool CResourceManager::LoadAnimationSequence(const std::string& Name,
	bool Loop, int StartFrame, int EndFrame, float PlayTime,
	const std::vector<BoneKeyFrame*>& vecFrame)
{
	return m_AnimationManager->LoadAnimationSequence(Name, Loop, 
		StartFrame, EndFrame, PlayTime, vecFrame);
}

bool CResourceManager::LoadAnimationSequence(
	const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	return m_AnimationManager->LoadAnimationSequence(Name, 
		FileName, PathName);
}

bool CResourceManager::LoadAnimationSequenceFullPath(
	const std::string& Name, const TCHAR* FullPath)
{
	return m_AnimationManager->LoadAnimationSequenceFullPath(Name, 
		FullPath);
}

bool CResourceManager::LoadAnimationSequenceMultibyte(
	const std::string& Name, const char* FileName, 
	const std::string& PathName)
{
	return m_AnimationManager->LoadAnimationSequenceMultibyte(Name, 
		FileName, PathName);
}

bool CResourceManager::LoadAnimationSequenceMultibyteFullPath(
	const std::string& Name, const char* FullPath)
{
	return m_AnimationManager->LoadAnimationSequenceMultibyteFullPath(Name, 
		FullPath);
}

CAnimationSequence* CResourceManager::FindAnimationSequence(
	const std::string& Name)
{
	return m_AnimationManager->FindAnimationSequence(Name);
}

void CResourceManager::ReleaseAnimationSequence(
	const std::string& Name)
{
	m_AnimationManager->ReleaseAnimationSequence(Name);
}

bool CResourceManager::LoadSkeleton(CScene* Scene, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	return m_AnimationManager->LoadSkeleton(Scene, Name, FileName, PathName);
}

bool CResourceManager::LoadSkeletonFullPath(
	CScene* Scene, const std::string& Name, const TCHAR* FullPath)
{
	return m_AnimationManager->LoadSkeletonFullPath(Scene, Name, FullPath);
}

bool CResourceManager::LoadSkeletonMultibyte(
	CScene* Scene, const std::string& Name,
	const char* FileName, const std::string& PathName)
{
	return m_AnimationManager->LoadSkeletonMultibyte(Scene, Name, FileName, PathName);
}

bool CResourceManager::LoadSkeletonMultibyteFullPath(
	CScene* Scene, const std::string& Name, const char* FullPath)
{
	return m_AnimationManager->LoadSkeletonMultibyteFullPath(Scene, Name, FullPath);
}

void CResourceManager::AddSocket(const std::string& SkeletonName, const std::string& BoneName, const std::string& SocketName, const Vector3& Offset, const Vector3& OffsetRot)
{
	m_AnimationManager->AddSocket(SkeletonName, BoneName, SocketName, Offset, OffsetRot);
}

bool CResourceManager::SetMeshSkeleton(const std::string& MeshName,
	const std::string& SkeletonName)
{
	CAnimationMesh* Mesh = (CAnimationMesh*)FindMesh(MeshName);

	if (!Mesh)
		return false;

	CSkeleton* Skeleton = FindSkeleton(SkeletonName);

	if (!Skeleton)
		return false;

	Mesh->SetSkeleton(Skeleton);

	return true;
}

CSkeleton* CResourceManager::FindSkeleton(const std::string& Name)
{
	return m_AnimationManager->FindSkeleton(Name);
}

void CResourceManager::ReleaseSkeleton(const std::string& Name)
{
	m_AnimationManager->ReleaseSkeleton(Name);
}

bool CResourceManager::CreateSoundChannel(const std::string& Name)
{
	return m_SoundManager->CreateSoundChannel(Name);
}

bool CResourceManager::LoadSound(const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName)
{
	return m_SoundManager->LoadSound(GroupName, Name, Loop, FileName, PathName);
}

bool CResourceManager::SetVolume(float Volume)
{
	return m_SoundManager->SetVolume(Volume);
}

bool CResourceManager::SetVolume(const std::string& GroupName, int Volume)
{
	return m_SoundManager->SetVolume(GroupName, Volume);
}

bool CResourceManager::SoundPlay(const std::string& Name)
{
	return m_SoundManager->SoundPlay(Name);
}

bool CResourceManager::SoundStop(const std::string& Name)
{
	return m_SoundManager->SoundStop(Name);
}

bool CResourceManager::SoundPause(const std::string& Name)
{
	return m_SoundManager->SoundPause(Name);
}

bool CResourceManager::SoundResume(const std::string& Name)
{
	return m_SoundManager->SoundResume(Name);
}

FMOD::ChannelGroup* CResourceManager::FindChannelGroup(const std::string& Name)
{
	return m_SoundManager->FindChannelGroup(Name);
}

CSound* CResourceManager::FindSound(const std::string& Name)
{
	return m_SoundManager->FindSound(Name);
}

void CResourceManager::ReleaseSound(const std::string& Name)
{
	m_SoundManager->ReleaseSound(Name);
}

bool CResourceManager::CreateFontCollection(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	return m_FontManager->CreateFontCollection(Name, FileName, PathName);
}

bool CResourceManager::LoadFont(const std::string& Name, const TCHAR* FontName, 
	int Weight, float FontSize, const TCHAR* LocalName, int Stretch)
{
	return m_FontManager->LoadFont(Name, FontName, Weight,
		FontSize, LocalName, Stretch);
}

const TCHAR* CResourceManager::GetFontFaceName(const std::string& CollectionName)
{
	return m_FontManager->GetFontFaceName(CollectionName);
}

const char* CResourceManager::GetFontFaceNameMultibyte(const std::string& CollectionName)
{
	return m_FontManager->GetFontFaceNameMultibyte(CollectionName);
}

bool CResourceManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(float r, float g, float b, float a)
{
	return m_FontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(const Vector4& Color)
{
	return m_FontManager->CreateFontColor(Color);
}

bool CResourceManager::CreateFontColor(unsigned int Color)
{
	return m_FontManager->CreateFontColor(Color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(float r, float g, float b, float a)
{
	return m_FontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const Vector4& Color)
{
	return m_FontManager->FindFontColor(Color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned int Color)
{
	return m_FontManager->FindFontColor(Color);
}

unsigned int CResourceManager::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->CreateFontColorKey(r, g, b, a);
}

unsigned int CResourceManager::CreateFontColorKey(float r, float g, float b, float a)
{
	return m_FontManager->CreateFontColorKey(r, g, b, a);
}

unsigned int CResourceManager::CreateFontColorKey(const Vector4& Color)
{
	return m_FontManager->CreateFontColorKey(Color);
}

CFont* CResourceManager::FindFont(const std::string& Name)
{
	return m_FontManager->FindFont(Name);
}

CFontCollection* CResourceManager::FindFontCollection(const std::string& Name)
{
	return m_FontManager->FindFontCollection(Name);
}

void CResourceManager::ReleaseFont(const std::string& Name)
{
	m_FontManager->ReleaseFont(Name);
}

void CResourceManager::ReleaseFontCollection(const std::string& Name)
{
	m_FontManager->ReleaseFontCollection(Name);
}

bool CResourceManager::CreateParticle(const std::string& Name)
{
	return m_ParticleManager->CreateParticle(Name);
}

CParticle* CResourceManager::FindParticle(const std::string& Name)
{
	return m_ParticleManager->FindParticle(Name);
}

void CResourceManager::ReleaseParticle(const std::string& Name)
{
	m_ParticleManager->ReleaseParticle(Name);
}
