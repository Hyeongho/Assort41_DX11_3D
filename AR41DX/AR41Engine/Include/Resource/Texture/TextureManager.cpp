
#include "TextureManager.h"
#include "Texture.h"
#include "../../Device.h"
#include "RenderTarget.h"
#include "../Shader/Shader.h"
#include "../ResourceManager.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
	auto	iter = m_mapSampler.begin();
	auto	iterEnd = m_mapSampler.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_RELEASE(iter->second);
	}
}

bool CTextureManager::Init()
{
	/*	
	Sampler는 3가지의 필터방식을 가진다.
	축소필터, 확대필터, Mip필터

	필터종류 : 점필터링, 선형필터링, 비등방성 필터링
	점필터링 : 속도가 제일 빠르다. 단, 품질은 떨어진다. 하지만 크기가 딱 맞아서
	확대나 축소가 안일어난다면 가장 좋은 선택지이다.

	선형필터링 : 속도 중간. 품질도 중간이다.

	비등방성 필터링 : 속도가 가장 느리다. 품질은 가장 좋다.
	*/

	float BorderColor[4] = { 1.f, 1.f,1.f, 1.f };

	if (FAILED(CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))
		return false;

	if (FAILED(CreateSampler("Linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))
		return false;

	if (FAILED(CreateSampler("Anisotropic", D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))

	// FXAA 용
	if (FAILED(CreateSampler("Clamp", D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP, BorderColor)))

	return false;

	SetSampler("Point", 0);
	SetSampler("Linear", 1);
	SetSampler("Anisotropic", 2);
	SetSampler("Clamp", 3); // FXAA 용

	m_DebugShader = CResourceManager::GetInst()->FindShader("DebugShader");

	if (!LoadTexture("EngineNoise", TEXT("noise_01.png")))
	{
		return false;
	}

	CTexture* Texture = FindTexture("EngineNoise");

	Texture->SetShader(6, (int)EShaderBufferType::All, 0);

	if (!LoadTexture("LUT", TEXT("LUT.png")))
	{
		return false;
	}

	Texture = FindTexture("LUT");

	Texture->SetShader(7, (int)EShaderBufferType::Pixel, 0);

	return true;
}

bool CTextureManager::LoadTexture(const std::string& Name, const TCHAR* FileName, 
	const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTextureFullPath(Name, FullPath))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTexture(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, vecFileName, PathName))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTextureFullPath(Name, vecFullPath))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureArray(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTextureArray(Name, vecFileName, PathName))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTextureArrayFullPath(Name, vecFullPath))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CTextureManager::CreateTarget(const std::string& Name, 
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat, 
	DXGI_FORMAT DepthFormat)
{
	CRenderTarget* Texture = (CRenderTarget*)FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CRenderTarget;

	if (!Texture->CreateTarget(Name, Width, Height, PixelFormat, DepthFormat))
	{
		SAFE_DELETE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

void CTextureManager::Render()
{
	auto	iter = m_mapTexture.begin();
	auto	iterEnd = m_mapTexture.end();

	for (; iter != iterEnd; iter++)
	{
		if (iter->second->GetImageType() == EImageType::RenderTarget)
		{
			m_DebugShader->SetShader();

			iter->second->Render();
		}
	}
}

CTexture* CTextureManager::FindTexture(const std::string& Name)
{
	auto	iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

void CTextureManager::ReleaseTexture(const std::string& Name)
{
	auto	iter = m_mapTexture.find(Name);

	if (iter != m_mapTexture.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapTexture.erase(iter);
	}
}

bool CTextureManager::CreateSampler(const std::string& Name, 
	D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU, 
	D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW,
	float BorderColor[4])
{
	ID3D11SamplerState* Sampler = FindSampler(Name);

	if (Sampler)
		return true;

	D3D11_SAMPLER_DESC	Desc = {};

	Desc.Filter = Filter;
	Desc.AddressU = AddressU;
	Desc.AddressV = AddressV;
	Desc.AddressW = AddressW;
	Desc.MipLODBias = 0.f;
	Desc.MaxAnisotropy = 1;
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;

	memcpy(Desc.BorderColor, BorderColor, sizeof(float) * 4);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&Desc, &Sampler)))
		return false;

	m_mapSampler.insert(std::make_pair(Name, Sampler));

	return true;
}

ID3D11SamplerState* CTextureManager::FindSampler(const std::string& Name)
{
	auto	iter = m_mapSampler.find(Name);

	if (iter == m_mapSampler.end())
		return nullptr;

	return iter->second;
}

void CTextureManager::SetSampler(const std::string& Name, int Register,
	int ShaderBufferType)
{
	ID3D11SamplerState* Sampler = FindSampler(Name);

	if (!Sampler)
		return;

	if (ShaderBufferType & (int)EShaderBufferType::Vertex)
		CDevice::GetInst()->GetContext()->VSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Pixel)
		CDevice::GetInst()->GetContext()->PSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Hull)
		CDevice::GetInst()->GetContext()->HSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Domain)
		CDevice::GetInst()->GetContext()->DSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Geometry)
		CDevice::GetInst()->GetContext()->GSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Compute)
		CDevice::GetInst()->GetContext()->CSSetSamplers(Register, 1, &Sampler);
}
