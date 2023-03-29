#include "RenderTarget.h"
#include "../../Device.h"
#include "../ResourceManager.h"
#include "../Mesh/Mesh.h"
#include "../../Engine.h"
#include "../Shader/GlobalConstantBuffer.h"

CRenderTarget::CRenderTarget()	:
	m_TargetView(nullptr),
	m_TargetTex(nullptr),
	m_DepthView(nullptr),
	m_PrevTargetView(nullptr),
	m_PrevDepthView(nullptr),
	m_Surface(nullptr),
	m_ClearColor{},
	m_DebugRender(true),
	m_CBuffer(nullptr),
	m_Scale(100.f, 100.f, 1.f)
{
	m_ImageType = EImageType::RenderTarget;
}

CRenderTarget::~CRenderTarget()
{
	SAFE_DELETE(m_CBuffer);

	SAFE_RELEASE(m_DepthView);
	SAFE_RELEASE(m_Surface);
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_TargetTex);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_PrevDepthView);
}

bool CRenderTarget::CreateTarget(const std::string& Name,
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat,
	DXGI_FORMAT DepthFormat)
{
	SetName(Name);

    int	SampleCount = CDevice::GetInst()->GetSampleCount(); 

	// Target侩 Texture 积己
	D3D11_TEXTURE2D_DESC	Desc = {};

	Desc.Width = Width;
	Desc.Height = Height;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = SampleCount;
	Desc.SampleDesc.Quality = 0;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.Format = PixelFormat;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&Desc, nullptr,
		&m_TargetTex)))
		return false;

	TextureResourceInfo* Info = new TextureResourceInfo;

	Info->Width = Width;
	Info->Height = Height;
	m_vecTextureInfo.push_back(Info);

	m_TargetTex->QueryInterface(__uuidof(IDXGISurface),
		(void**)&m_Surface);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(
		m_TargetTex, nullptr, &Info->SRV)))
		return false;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(m_TargetTex,
		nullptr, &m_TargetView)))
		return false;

	if (DepthFormat != DXGI_FORMAT_UNKNOWN)
	{
		D3D11_TEXTURE2D_DESC	DepthDesc = {};

		DepthDesc.Width = Width;
		DepthDesc.Height = Height;
		DepthDesc.ArraySize = 1;
		DepthDesc.MipLevels = 1;
		DepthDesc.SampleDesc.Count = SampleCount;
		DepthDesc.SampleDesc.Quality = 0;
		DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthDesc.Format = DepthFormat;
		DepthDesc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* DepthTex = nullptr;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&DepthDesc, nullptr,
			&DepthTex)))
			return false;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilView(DepthTex,
			nullptr, &m_DepthView)))
			return false;

		SAFE_RELEASE(DepthTex);
	}

	m_CBuffer = new CTransformConstantBuffer;

	m_CBuffer->Init();

	m_matProj	= DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)Width, 0.f,
		(float)Height, 0.f, 1000.f);

	m_CBuffer->SetProjMatrix(m_matProj);

	m_Mesh = CResourceManager::GetInst()->FindMesh("LBUVRect");

	return true;
}

bool CRenderTarget::CreateTargetNoMS(const std::string& Name,
	unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat,
	DXGI_FORMAT DepthFormat)
{
	SetName(Name);

	int	SampleCount = CDevice::GetInst()->GetSampleCount(); //0325

	// Target侩 Texture 积己
	D3D11_TEXTURE2D_DESC	Desc = {};

	Desc.Width = Width;
	Desc.Height = Height;
	Desc.ArraySize = 1;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Desc.Format = PixelFormat;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&Desc, nullptr,
		&m_TargetTex)))
		return false;

	TextureResourceInfo* Info = new TextureResourceInfo;

	Info->Width = Width;
	Info->Height = Height;
	m_vecTextureInfo.push_back(Info);

	m_TargetTex->QueryInterface(__uuidof(IDXGISurface),
		(void**)&m_Surface);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(
		m_TargetTex, nullptr, &Info->SRV)))
		return false;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(m_TargetTex,
		nullptr, &m_TargetView)))
		return false;

	if (DepthFormat != DXGI_FORMAT_UNKNOWN)
	{
		D3D11_TEXTURE2D_DESC	DepthDesc = {};

		DepthDesc.Width = Width;
		DepthDesc.Height = Height;
		DepthDesc.ArraySize = 1;
		DepthDesc.MipLevels = 1;
		DepthDesc.SampleDesc.Count = 1;
		DepthDesc.SampleDesc.Quality = 0;
		DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthDesc.Format = DepthFormat;
		DepthDesc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Texture2D* DepthTex = nullptr;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&DepthDesc, nullptr,
			&DepthTex)))
			return false;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilView(DepthTex,
			nullptr, &m_DepthView)))
			return false;

		SAFE_RELEASE(DepthTex);
	}

	m_CBuffer = new CTransformConstantBuffer;

	m_CBuffer->Init();

	m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)Width, 0.f,
		(float)Height, 0.f, 1000.f);

	m_CBuffer->SetProjMatrix(m_matProj);

	m_Mesh = CResourceManager::GetInst()->FindMesh("LBUVRect");

	return true;
}

void CRenderTarget::ClearTarget()
{
	CDevice::GetInst()->GetContext()->ClearRenderTargetView(m_TargetView,
		m_ClearColor);

	if (m_DepthView)
	{
		CDevice::GetInst()->GetContext()->ClearDepthStencilView(m_DepthView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.f, 0);
	}
}

void CRenderTarget::SetTarget(ID3D11DepthStencilView* DepthView)
{
	CDevice::GetInst()->GetContext()->OMGetRenderTargets(1, &m_PrevTargetView,
		&m_PrevDepthView);

	ID3D11DepthStencilView* Depth = m_PrevDepthView;

	if (DepthView)
		Depth = DepthView;

	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_TargetView, Depth);
}

void CRenderTarget::SetTarget()
{
	CDevice::GetInst()->GetContext()->OMGetRenderTargets(1, &m_PrevTargetView,
		&m_PrevDepthView);

	ID3D11DepthStencilView* Depth = m_PrevDepthView;

	if (m_DepthView)
		Depth = m_DepthView;

	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_TargetView, Depth);
}

void CRenderTarget::ResetTarget()
{
	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_PrevTargetView,
		m_PrevDepthView);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_PrevDepthView);
}

// Debug侩
void CRenderTarget::SetTargetShader()
{
	CDevice::GetInst()->GetContext()->PSSetShaderResources(10, 1, &m_vecTextureInfo[0]->SRV);
}

void CRenderTarget::ResetTargetShader()
{
	ID3D11ShaderResourceView* SRV = nullptr;
	CDevice::GetInst()->GetContext()->PSSetShaderResources(10, 1, &SRV);
}

void CRenderTarget::SetTargetShader(int Register)
{
	CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &m_vecTextureInfo[0]->SRV);
}

void CRenderTarget::ResetTargetShader(int Register)
{
	ID3D11ShaderResourceView* SRV = nullptr;
	CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &SRV);
}

void CRenderTarget::Render()
{
	m_CBuffer->SetWorldMatrix(m_matDebugWVP);
	m_CBuffer->SetProjMatrix(m_matProj);

	m_CBuffer->UpdateBuffer();

	CGlobalConstantBuffer* Buffer = CEngine::GetInst()->GetGlobalCBuffer();

	Buffer->SetShadowMapResolution((float)GetWidth(), (float)GetHeight());

	Buffer->UpdateBuffer();

	SetTargetShader();

	m_Mesh->Render();

	ResetTargetShader();
}
