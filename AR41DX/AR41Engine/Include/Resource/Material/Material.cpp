
#include "Material.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../Shader/MaterialConstantBuffer.h"
#include "../Shader/Shader.h"
#include "../../Render/RenderManager.h"

CMaterial::CMaterial() :
	m_BaseColor(Vector4::White),
	m_AmbientColor(0.2f, 0.2f, 0.2f, 1.f),
	m_SpecularColor(Vector4::White),
	m_EmissiveColor(Vector4::Black),
	m_Opacity(1.f),
	m_Animation3D(false)
{
	SetTypeID<CMaterial>();

	m_CBuffer = new CMaterialConstantBuffer;

	m_CBuffer->Init();

	m_CBuffer->SetBaseColor(m_BaseColor);
	m_CBuffer->SetAmbientColor(m_AmbientColor);
	m_CBuffer->SetSpecularColor(m_SpecularColor);
	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
	m_CBuffer->SetOpacity(m_Opacity);
}

CMaterial::CMaterial(const CMaterial& Material) :
	CRef(Material)
{
	m_Shader = Material.m_Shader;
	m_BaseColor = Material.m_BaseColor;
	m_AmbientColor = Material.m_AmbientColor;
	m_SpecularColor = Material.m_SpecularColor;
	m_EmissiveColor = Material.m_EmissiveColor;
	m_Opacity = Material.m_Opacity;
	m_Animation3D = Material.m_Animation3D;

	m_CBuffer = Material.m_CBuffer->Clone();

	auto	iter = Material.m_vecTextureInfo.begin();
	auto	iterEnd = Material.m_vecTextureInfo.end();

	for (; iter != iterEnd; iter++)
	{
		MaterialTextureInfo* Info = new MaterialTextureInfo;

		Info->Name = (*iter)->Name;
		Info->SamplerType = (*iter)->SamplerType;
		Info->Texture = (*iter)->Texture;
		Info->Register = (*iter)->Register;
		Info->ShaderBufferType = (*iter)->ShaderBufferType;

		m_vecTextureInfo.push_back(Info);
	}

	m_Scene = nullptr;

	for (int i = 0; i < 3; i++)
	{
		if (Material.m_RenderState[i])
			m_RenderState[i] = Material.m_RenderState[i];
	}
}

CMaterial::~CMaterial()
{
	auto	iter = m_vecTextureInfo.begin();
	auto	iterEnd = m_vecTextureInfo.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE((*iter));
	}

	SAFE_DELETE(m_CBuffer);
}

void CMaterial::SetBaseColor(const Vector4& Color)
{
	m_BaseColor = Color;

	m_CBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	m_BaseColor = Vector4(r, g, b, a);

	m_CBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetBaseColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_BaseColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_CBuffer->SetBaseColor(m_BaseColor);
}

void CMaterial::SetAmbientColor(const Vector4& Color)
{
	m_AmbientColor = Color;

	m_CBuffer->SetAmbientColor(m_AmbientColor);
}

void CMaterial::SetAmbientColor(float r, float g, float b, float a)
{
	m_AmbientColor = Vector4(r, g, b, a);

	m_CBuffer->SetAmbientColor(m_AmbientColor);
}

void CMaterial::SetAmbientColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_AmbientColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_CBuffer->SetAmbientColor(m_AmbientColor);
}

void CMaterial::SetSpecularColor(const Vector4& Color)
{
	m_SpecularColor.x = Color.x;
	m_SpecularColor.y = Color.y;
	m_SpecularColor.z = Color.z;

	m_CBuffer->SetSpecularColor(m_SpecularColor);
}

void CMaterial::SetSpecularColor(float r, float g, float b)
{
	m_SpecularColor = Vector4(r, g, b, m_SpecularColor.w);

	m_CBuffer->SetSpecularColor(m_SpecularColor);
}

void CMaterial::SetSpecularColorUnsignedChar(unsigned char r,
	unsigned char g, unsigned char b)
{
	m_SpecularColor = Vector4(r / 255.f, g / 255.f, b / 255.f, m_SpecularColor.w);

	m_CBuffer->SetSpecularColor(m_SpecularColor);
}

void CMaterial::SetSpecularPower(float Power)
{
	m_SpecularColor.w = Power;

	m_CBuffer->SetSpecularColor(m_SpecularColor);
}

void CMaterial::SetEmissiveColor(const Vector4& Color)
{
	m_EmissiveColor = Color;

	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
}

void CMaterial::SetEmissiveColor(float r, float g, float b, float a)
{
	m_EmissiveColor = Vector4(r, g, b, a);

	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
}

void CMaterial::SetEmissiveColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_EmissiveColor = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);

	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
}

void CMaterial::SetOpacity(float Opacity)
{
	m_Opacity = Opacity;

	m_CBuffer->SetOpacity(m_Opacity);
}

void CMaterial::AddOpacity(float Opacity)
{
	m_Opacity += Opacity;

	m_CBuffer->SetOpacity(m_Opacity);
}

CRenderState* CMaterial::GetRenderState(int index) const
{
	size_t size = std::size(m_RenderState);
	if ((int)size <= index)
	{
		return nullptr;
	}
	return m_RenderState[index];
}

bool CMaterial::GetIsBump()
{
	return m_CBuffer->GetIsBump();
}

bool CMaterial::GetIsSpecular()
{
	return m_CBuffer->GetIsSpecular();
}

bool CMaterial::GetIsEmissive()
{
	return m_CBuffer->GetIsEmissive();
}

bool CMaterial::GetIsAnimation3D()
{
	return m_Animation3D;
}

void CMaterial::EnableBump()
{
	m_CBuffer->SetEnableBump(true);
}

void CMaterial::EnableSpecular()
{
	m_CBuffer->SetEnableSpecular(true);
}

void CMaterial::EnableEmissive()
{
	m_CBuffer->SetEnableEmissive(true);
}

void CMaterial::EnableAnimation3D()
{
	m_Animation3D = true;
	m_CBuffer->SetAnimation3D(true);
}

void CMaterial::UnEnableBump()
{
	m_CBuffer->SetEnableBump(false);
}

void CMaterial::UnEnableSpecular()
{
	m_CBuffer->SetEnableSpecular(false);
}

void CMaterial::UnEnableEmissive()
{
	m_CBuffer->SetEnableEmissive(false);
}

void CMaterial::UnEnableAnimation3D()
{
	m_Animation3D = false;
	m_CBuffer->SetAnimation3D(false);
}

void CMaterial::SetReceiveDecal(bool receive)
{
	m_ReceiveDecal = receive;
	//m_CBuffer->SetReceiveDecal(receive);
}

void CMaterial::AddTexture(int Register, int ShaderBufferType,
	const std::string& Name, CTexture* Texture)
{
	MaterialTextureInfo* Info = new MaterialTextureInfo;

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;
	Info->Texture = Texture;

	m_vecTextureInfo.push_back(Info);
}

void CMaterial::AddTexture(int Register, int ShaderBufferType,
	const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	MaterialTextureInfo* Info = new MaterialTextureInfo;

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_vecTextureInfo.push_back(Info);
}

void CMaterial::AddTextureFullPath(int Register, int ShaderBufferType,
	const std::string& Name, const TCHAR* FullPath)
{
	MaterialTextureInfo* Info = new MaterialTextureInfo;

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(Name, FullPath))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_vecTextureInfo.push_back(Info);
}

void CMaterial::AddTexture(int Register, int ShaderBufferType,
	const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	MaterialTextureInfo* Info = new MaterialTextureInfo;

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, vecFileName, PathName))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_vecTextureInfo.push_back(Info);
}

void CMaterial::AddTextureFullPath(int Register, int ShaderBufferType,
	const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	MaterialTextureInfo* Info = new MaterialTextureInfo;

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(Name, vecFullPath))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_vecTextureInfo.push_back(Info);
}

void CMaterial::AddTextureArray(int Register, int ShaderBufferType,
	const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	MaterialTextureInfo* Info = new MaterialTextureInfo;

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArray(Name, vecFileName, PathName))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArray(Name, vecFileName, PathName))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_vecTextureInfo.push_back(Info);
}

void CMaterial::AddTextureArrayFullPath(int Register, int ShaderBufferType,
	const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	MaterialTextureInfo* Info = new MaterialTextureInfo;

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArrayFullPath(Name, vecFullPath))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(Name, vecFullPath))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	m_vecTextureInfo.push_back(Info);
}

void CMaterial::SetTexture(int Index, int Register, int ShaderBufferType,
	const std::string& Name, CTexture* Texture)
{
	if (Index < 0 || Index >= (int)m_vecTextureInfo.size())
		return;

	MaterialTextureInfo* Info = m_vecTextureInfo[Index];

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;
	Info->Texture = Texture;
}

void CMaterial::SetTexture(int Index, int Register, int ShaderBufferType,
	const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (Index < 0 || Index >= (int)m_vecTextureInfo.size())
		return;

	MaterialTextureInfo* Info = m_vecTextureInfo[Index];

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}
}

void CMaterial::SetTextureFullPath(int Index, int Register, int ShaderBufferType, const std::string& Name, const TCHAR* FullPath)
{
	if (Index < 0 || Index >= (int)m_vecTextureInfo.size())
		return;

	MaterialTextureInfo* Info = m_vecTextureInfo[Index];

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(Name, FullPath))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}
}

void CMaterial::SetTexture(int Index, int Register, int ShaderBufferType,
	const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	if (Index < 0 || Index >= (int)m_vecTextureInfo.size())
		return;

	MaterialTextureInfo* Info = m_vecTextureInfo[Index];

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, vecFileName, PathName))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}
}

void CMaterial::SetTextureFullPath(int Index, int Register,
	int ShaderBufferType, const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (Index < 0 || Index >= (int)m_vecTextureInfo.size())
		return;

	MaterialTextureInfo* Info = m_vecTextureInfo[Index];

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureFullPath(Name, vecFullPath))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}
}

void CMaterial::SetTextureArray(int Index, int Register,
	int ShaderBufferType, const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	if (Index < 0 || Index >= (int)m_vecTextureInfo.size())
		return;

	MaterialTextureInfo* Info = m_vecTextureInfo[Index];

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArray(Name, vecFileName, PathName))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArray(Name, vecFileName, PathName))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}
}

void CMaterial::SetTextureArrayFullPath(int Index, int Register,
	int ShaderBufferType, const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (Index < 0 || Index >= (int)m_vecTextureInfo.size())
		return;

	MaterialTextureInfo* Info = m_vecTextureInfo[Index];

	Info->Register = Register;
	Info->ShaderBufferType = ShaderBufferType;
	Info->Name = Name;

	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTextureArrayFullPath(Name, vecFullPath))
			return;

		Info->Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(Name, vecFullPath))
			return;

		Info->Texture = CResourceManager::GetInst()->FindTexture(Name);
	}
}

void CMaterial::SetTextureSamplerType(int Index, ESamplerType Type)
{
	if (Index < 0 || Index >= (int)m_vecTextureInfo.size())
		return;

	MaterialTextureInfo* Info = m_vecTextureInfo[Index];

	Info->SamplerType = Type;
}

void CMaterial::SetTextureFrameIndex(int TexIndex, int FrameIndex)
{
	m_vecTextureInfo[TexIndex]->Index = FrameIndex;
}

CTexture* CMaterial::GetTexture(int Index) const
{
	if ((int)m_vecTextureInfo.size() <= Index)
		return nullptr;

	return m_vecTextureInfo[Index]->Texture;
}

void CMaterial::SetRenderState(const std::string& Name)
{
	CRenderState* RenderState = CRenderManager::GetInst()->FindRenderState<CRenderState>(Name);

	if (!RenderState)
		return;

	m_RenderState[(int)RenderState->GetType()] = RenderState;
}

void CMaterial::CopyInstancingData(InstancingBufferInfo& Data)	const
{
	Data.BaseColor = m_BaseColor;
	Data.AmbientColor = m_AmbientColor;
	Data.SpecularColor = m_SpecularColor;
	Data.EmissiveColor = m_EmissiveColor;
	Data.Opacity = m_Opacity;
	Data.Animation3D = m_Animation3D ? 1 : 0;
}

void CMaterial::SetShader(const std::string& Name)
{
	if (m_Scene)
	{
		m_Shader = m_Scene->GetResource()->FindShader(Name);
	}

	else
	{
		m_Shader = CResourceManager::GetInst()->FindShader(Name);
	}
}

void CMaterial::SetMaterial()
{
	if (m_Shader)
		m_Shader->SetShader();

	for (int i = 0; i < 3; ++i)
	{
		if (m_RenderState[i])
			m_RenderState[i]->SetState();
	}

	if (!m_vecTextureInfo.empty())
	{
		m_CBuffer->SetImageType(m_vecTextureInfo[0]->Texture->GetImageType());

		m_CBuffer->SetTextureWidth((float)m_vecTextureInfo[0]->Texture->GetWidth());
		m_CBuffer->SetTextureHeight((float)m_vecTextureInfo[0]->Texture->GetHeight());
	}


	m_CBuffer->UpdateBuffer();

	size_t	Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTextureInfo[i]->Texture->SetShader(m_vecTextureInfo[i]->Register,
			m_vecTextureInfo[i]->ShaderBufferType,
			m_vecTextureInfo[i]->Index);
	}
}

void CMaterial::ResetMaterial()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_RenderState[i])
			m_RenderState[i]->ResetState();
	}

	size_t	Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTextureInfo[i]->Texture->ResetShader(m_vecTextureInfo[i]->Register,
			m_vecTextureInfo[i]->ShaderBufferType);
	}
}

void CMaterial::SetShadowMaterial()
{
	m_CBuffer->UpdateBuffer();
}

void CMaterial::SetInstancingMaterial()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_RenderState[i])
			m_RenderState[i]->SetState();
	}

	size_t	Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTextureInfo[i]->Texture->SetShader(m_vecTextureInfo[i]->Register,
			m_vecTextureInfo[i]->ShaderBufferType,
			m_vecTextureInfo[i]->Index);
	}
}

void CMaterial::ResetInstancingMaterial()
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_RenderState[i])
			m_RenderState[i]->ResetState();
	}

	size_t	Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecTextureInfo[i]->Texture->ResetShader(m_vecTextureInfo[i]->Register, m_vecTextureInfo[i]->ShaderBufferType);
	}
}

CMaterial* CMaterial::Clone() const
{
	return new CMaterial(*this);
}

void CMaterial::Save(FILE* File)
{
	CRef::Save(File);

	int Length = (int)m_Shader->GetName().length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_Shader->GetName().c_str(), 1, Length, File);

	fwrite(&m_BaseColor, sizeof(Vector4), 1, File);
	fwrite(&m_AmbientColor, sizeof(Vector4), 1, File);
	fwrite(&m_SpecularColor, sizeof(Vector4), 1, File);
	fwrite(&m_EmissiveColor, sizeof(Vector4), 1, File);
	fwrite(&m_Opacity, 4, 1, File);
	fwrite(&m_Animation3D, 4, 1, File);

	for (int i = 0; i < 3; ++i)
	{
		bool	RenderStateEnable = false;

		if (m_RenderState[i])
			RenderStateEnable = true;

		fwrite(&RenderStateEnable, 1, 1, File);

		if (RenderStateEnable)
		{
			Length = (int)m_RenderState[i]->GetName().length();

			fwrite(&Length, 4, 1, File);
			fwrite(m_RenderState[i]->GetName().c_str(), 1, Length, File);
		}
	}

	int	TextureCount = (int)m_vecTextureInfo.size();
	fwrite(&TextureCount, 4, 1, File);

	for (int i = 0; i < TextureCount; ++i)
	{
		Length = (int)m_vecTextureInfo[i]->Name.length();

		fwrite(&Length, 4, 1, File);
		fwrite(m_vecTextureInfo[i]->Name.c_str(), 1, Length, File);

		fwrite(&m_vecTextureInfo[i]->SamplerType, sizeof(ESamplerType), 1, File);
		fwrite(&m_vecTextureInfo[i]->Register, sizeof(int), 1, File);
		fwrite(&m_vecTextureInfo[i]->ShaderBufferType, sizeof(int), 1, File);

		m_vecTextureInfo[i]->Texture->Save(File);
	}
}

void CMaterial::Load(FILE* File)
{
	CRef::Load(File);

	int Length = 0;
	char	ShaderName[256] = {};

	fread(&Length, 4, 1, File);
	fread(ShaderName, 1, Length, File);

	SetShader(ShaderName);

	fread(&m_BaseColor, sizeof(Vector4), 1, File);
	fread(&m_AmbientColor, sizeof(Vector4), 1, File);
	fread(&m_SpecularColor, sizeof(Vector4), 1, File);
	fread(&m_EmissiveColor, sizeof(Vector4), 1, File);
	fread(&m_Opacity, 4, 1, File);
	fread(&m_Animation3D, 4, 1, File);


	m_CBuffer->SetBaseColor(m_BaseColor);
	m_CBuffer->SetAmbientColor(m_AmbientColor);
	m_CBuffer->SetSpecularColor(m_SpecularColor);
	m_CBuffer->SetEmissiveColor(m_EmissiveColor);
	m_CBuffer->SetOpacity(m_Opacity);
	m_CBuffer->SetAnimation3D(m_Animation3D);

	for (int i = 0; i < 3; ++i)
	{
		bool	RenderStateEnable = false;

		fread(&RenderStateEnable, 1, 1, File);

		if (RenderStateEnable)
		{
			Length = 0;
			char	RenderStateName[256] = {};

			fread(&Length, 4, 1, File);
			fread(RenderStateName, 1, Length, File);

			SetRenderState(RenderStateName);
		}
	}

	int	TextureCount = 0;
	fread(&TextureCount, 4, 1, File);

	for (size_t i = 0; i < m_vecTextureInfo.size(); ++i)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}

	m_vecTextureInfo.clear();

	for (int i = 0; i < TextureCount; ++i)
	{
		MaterialTextureInfo* Info = new MaterialTextureInfo;

		m_vecTextureInfo.push_back(Info);

		Length = 0;
		char	TextureName[256] = {};

		fread(&Length, 4, 1, File);
		fread(TextureName, 1, Length, File);

		Info->Name = TextureName;

		fread(&Info->SamplerType, sizeof(ESamplerType), 1, File);
		fread(&Info->Register, sizeof(int), 1, File);
		fread(&Info->ShaderBufferType, sizeof(int), 1, File);

		EImageType	ImageType;
		fread(&ImageType, sizeof(EImageType), 1, File);

		int	TextureSRVCount = 0;

		fread(&TextureSRVCount, sizeof(int), 1, File);

		TCHAR	FolderName[8] = {};
		lstrcpy(FolderName, TEXT("Texture"));

		if (TextureSRVCount == 1)
		{
			TCHAR	FileName[MAX_PATH] = {};
			char	PathName[MAX_PATH] = {};

			fread(FileName, sizeof(TCHAR), MAX_PATH, File);
			fread(PathName, sizeof(char), MAX_PATH, File);

			if (m_Scene)
			{
				m_Scene->GetResource()->LoadTexture(Info->Name, FileName, PathName);

				Info->Texture = m_Scene->GetResource()->FindTexture(Info->Name);
			}

			else
			{
				CResourceManager::GetInst()->LoadTexture(Info->Name, FileName, PathName);

				Info->Texture = CResourceManager::GetInst()->FindTexture(Info->Name);
			}
		}

		else
		{
			if (ImageType == EImageType::Frame)
			{
				std::vector<const TCHAR*>	vecFileName;
				std::string	ResultPathName;

				for (int i = 0; i < TextureSRVCount; ++i)
				{
					TCHAR* FileName = new TCHAR[MAX_PATH];
					char	PathName[MAX_PATH] = {};

					fread(FileName, sizeof(TCHAR), MAX_PATH, File);
					fread(PathName, sizeof(char), MAX_PATH, File);

					ResultPathName = PathName;

					vecFileName.push_back(FileName);
				}

				if (m_Scene)
				{
					m_Scene->GetResource()->LoadTexture(Info->Name, vecFileName, ResultPathName);

					Info->Texture = m_Scene->GetResource()->FindTexture(Info->Name);
				}

				else
				{
					CResourceManager::GetInst()->LoadTexture(Info->Name, vecFileName, ResultPathName);

					Info->Texture = CResourceManager::GetInst()->FindTexture(Info->Name);
				}

				for (int i = 0; i < TextureSRVCount; ++i)
				{
					SAFE_DELETE_ARRAY(vecFileName[i]);
				}
			}

			else
			{
				std::vector<const TCHAR*>	vecFileName;
				std::string	ResultPathName;

				for (int i = 0; i < TextureSRVCount; ++i)
				{
					TCHAR* FileName = new TCHAR[MAX_PATH];
					char	PathName[MAX_PATH] = {};

					fread(FileName, sizeof(TCHAR), MAX_PATH, File);
					fread(PathName, sizeof(char), MAX_PATH, File);

					ResultPathName = PathName;

					vecFileName.push_back(FileName);
				}

				if (m_Scene)
				{
					m_Scene->GetResource()->LoadTextureArray(Info->Name, vecFileName, ResultPathName);

					Info->Texture = m_Scene->GetResource()->FindTexture(Info->Name);
				}

				else
				{
					CResourceManager::GetInst()->LoadTextureArray(Info->Name, vecFileName, ResultPathName);

					Info->Texture = CResourceManager::GetInst()->FindTexture(Info->Name);
				}

				for (int i = 0; i < TextureSRVCount; ++i)
				{
					SAFE_DELETE_ARRAY(vecFileName[i]);
				}
			}
		}
	}
}
