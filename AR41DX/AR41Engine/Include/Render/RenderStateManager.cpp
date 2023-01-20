#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

CRenderStateManager::CRenderStateManager()
{
}

CRenderStateManager::~CRenderStateManager()
{
}

bool CRenderStateManager::Init()
{
	AddBlendInfo("AlphaBlend");
	CreateBlendState("AlphaBlend", true, false);

	AddBlendInfo("LightAccBlend", true, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	CreateBlendState("LightAccBlend", true, false);

	CreateDepthStencil("DepthDisable", false);

	CreateDepthStencil("DepthLessEqual", true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);

	CreateDepthStencil("DepthWriteDisable", true, D3D11_DEPTH_WRITE_MASK_ZERO);

	CreateState("FrontFaceCull", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_FRONT);

	return true;
}

void CRenderStateManager::SetBlendFactor(const std::string& Name, float r, float g, float b, float a)
{
	CBlendState* State = FindRenderState<CBlendState>(Name);

	if (!State)
	{
		State = new CBlendState;

		State->SetName(Name);

		m_mapState.insert(std::make_pair(Name, State));
	}

	State->SetBlendFactor(r, g, b, a);
}

void CRenderStateManager::AddBlendInfo(const std::string& Name, bool BlendEnable,
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp,
	D3D11_BLEND SrcAlphBlend, D3D11_BLEND DestAlphBlend, D3D11_BLEND_OP BlendAlphOp,
	UINT8 WriteMask)
{
	CBlendState* State = FindRenderState<CBlendState>(Name);

	if (!State)
	{
		State = new CBlendState;

		State->SetName(Name);

		m_mapState.insert(std::make_pair(Name, State));
	}

	State->AddBlendInfo(BlendEnable, SrcBlend, DestBlend, BlendOp, SrcAlphBlend,
		DestAlphBlend, BlendAlphOp, WriteMask);
}

bool CRenderStateManager::CreateBlendState(const std::string& Name, 
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	CBlendState* State = FindRenderState<CBlendState>(Name);

	if (!State)
	{
		return false;
	}

	if (!State->CreateState(AlphaToCoverageEnable, IndependentBlendEnable))
	{
		SAFE_RELEASE(State);
		return false;
	}

	return true;
}

bool CRenderStateManager::CreateDepthStencil(const std::string& Name, 
	bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, 
	D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, UINT8 StencilReadMask, 
	UINT8 StencilWriteMask, D3D11_DEPTH_STENCILOP_DESC FrontFace,
	D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	CDepthStencilState* State = FindRenderState<CDepthStencilState>(Name);

	if (State)
	{
		return true;
	}

	State = new CDepthStencilState;

	State->SetName(Name);

	if (!State->CreateDepthStencil(DepthEnable, DepthWriteMask, DepthFunc,
		StencilEnable, StencilReadMask, StencilWriteMask, FrontFace,
		BackFace))
	{
		SAFE_RELEASE(State);
		return false;
	}

	m_mapState.insert(std::make_pair(Name, State));

	return true;
}

bool CRenderStateManager::CreateState(const std::string& Name,
	D3D11_FILL_MODE FillMode, D3D11_CULL_MODE CullMode,
	BOOL FrontCounterClockwise, INT DepthBias, FLOAT DepthBiasClamp,
	FLOAT SlopeScaledDepthBias, BOOL DepthClipEnable, BOOL ScissorEnable,
	BOOL MultisampleEnable, BOOL AntialiasedLineEnable)
{
	CRasterizerState* State = FindRenderState<CRasterizerState>(Name);

	if (State)
	{
		return true;
	}

	State = new CRasterizerState;

	State->SetName(Name);

	if (!State->CreateState(FillMode, CullMode, FrontCounterClockwise,
		DepthBias, DepthBiasClamp, SlopeScaledDepthBias, DepthClipEnable,
		ScissorEnable, MultisampleEnable, AntialiasedLineEnable))
	{
		SAFE_RELEASE(State);
		return false;
	}

	m_mapState.insert(std::make_pair(Name, State));

	return true;
}
