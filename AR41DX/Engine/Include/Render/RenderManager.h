#pragma once

#include "../EngineInfo.h"
#include "RenderStateManager.h"
#include "../Resource/Texture/RenderTarget.h"
#include "../Resource/Shader/GraphicShader.h"
#include "RenderInstancing.h"

struct RenderLayer
{
	std::string		Name;
	int				LayerPriority;
	CSharedPtr<class CRenderState> AlphaBlend;
	std::list<CSharedPtr<class CSceneComponent>>	RenderList;
};

class CRenderManager
{
private:
	std::vector<RenderLayer*>	m_RenderLayerList;
	CRenderStateManager* m_RenderStateManager;
	CSharedPtr<class CRenderState> m_AlphaBlend;
	CSharedPtr<class CRenderState> m_MRTAlphaBlend;
	CSharedPtr<class CRenderState> m_DepthDisable;
	CSharedPtr<class CRenderState> m_DepthWriteDisable;
	CSharedPtr<class CRenderState> m_LightAccBlend;

	std::vector<CSharedPtr<CRenderTarget>>	m_vecGBuffer;

	std::vector<CSharedPtr<CRenderTarget>>	m_vecDecalBuffer;

	std::vector<CSharedPtr<CRenderTarget>>	m_vecLightBuffer;

	CSharedPtr<CRenderTarget> m_ScreenBuffer;
	CSharedPtr<CRenderTarget> m_FXAABuffer; // FXAA ¹öÆÛ
	CSharedPtr<CRenderTarget> m_MSBuffer;
	CSharedPtr<CRenderTarget> m_CartoonBuffer;

	CSharedPtr<CGraphicShader> m_ScreenShader;
	CSharedPtr<CGraphicShader> m_DeferredRenderShader;
	CSharedPtr<CGraphicShader> m_FXAAShader; // FXAA
	CSharedPtr<CGraphicShader> m_MSShader;
	CSharedPtr<CGraphicShader> m_CartoonShader;


	class CTranslationConstantBuffer* m_TranslationCBuffer;


	CSharedPtr<CRenderTarget> m_ShadowMapTarget;

	Resolution m_ShadowMapRS;
	class CShadowConstantBuffer* m_ShadowCBuffer;


	std::list<CSharedPtr<class CSceneComponent>> m_NormalRenderList;


	std::vector<CRenderInstancing*>	m_vecInstancingPool;
	std::list<int> m_EmptyPoolList;
	std::unordered_map<class CMesh*, CRenderInstancing*> m_mapInstancing;

	EShaderType	m_ShaderType;

public:
	void SetShaderType(EShaderType Type);

public:
	const Resolution& GetShadowMapResolution()	const
	{
		return m_ShadowMapRS;
	}

	CRenderInstancing* FindInstancing(class CMesh* Mesh);

public:
	void CreateLayer(const std::string& Name, int Priority);
	void SetLayerPriority(const std::string& Name, int Priority);
	void SetLayerAlphaBlend(const std::string& Name);
	void DeleteLayer(const std::string& Name);
	void AddRenderList(class CSceneComponent* Component);

public:
	bool Init();
	void Render(float DeltaTime);

private:
	void Render3D(float DeltaTime);
	void RenderShadowMap(float DeltaTime);
	void RenderGBuffer(float DeltaTime);
	void RenderDecal(float DeltaTime);
	void RenderLight(float DeltaTime);
	void RenderScreen(float DeltaTime);
	void RenderCartoon(float DeltaTime);
	void RenderFXAA(float DeltaTime); // FXAA
	void RenderDeferred(float DeltaTime);
	void RenderParticle(float DeltaTime);
	void RenderTranslation(float DeltaTime);
	void RenderMultiSampling(float DeltaTime);

public:
	void SetBlendFactor(const std::string& Name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& Name, bool BlendEnable = true,
		D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcAlphBlend = D3D11_BLEND_ONE,
		D3D11_BLEND DestAlphBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendAlphOp = D3D11_BLEND_OP_ADD,
		UINT8 WriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable);

public:
	bool CreateDepthStencil(const std::string& Name, bool DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		bool StencilEnable = false,
		UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC FrontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC BackFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });

public:
	template <typename T>
	T* FindRenderState(const std::string& Name)
	{
		return m_RenderStateManager->FindRenderState<T>(Name);
	}

private:
	static bool SortLayer(RenderLayer* Src, RenderLayer* Dest);
	static bool SortAlphaObject(class CSceneComponent* Src, class CSceneComponent* Dest);
	float	m_TextureTranslation;

public:
	void CreateRenderTarget();

private:
	RenderLayer* FindLayer(const std::string& Name);

	DECLARE_SINGLE(CRenderManager)
};

