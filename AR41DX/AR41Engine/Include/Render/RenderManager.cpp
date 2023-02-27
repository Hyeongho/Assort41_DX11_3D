#include "RenderManager.h"
#include "../Component/SceneComponent.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneViewport.h"
#include "../Scene/LightManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "../Device.h"
#include "../Resource/ResourceManager.h"
#include "../Engine.h"
#include "../Resource/Mesh/Mesh.h"
#include "../GameObject/SkySphere.h"
#include "../Component/CameraComponent.h"
#include "../Component/ParticleComponent.h"
#include "../Component/DecalComponent.h"
#include "../Resource/Shader/ShadowConstantBuffer.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() : m_RenderStateManager(nullptr), m_ShaderType(EShaderType::Default)
{
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(m_ShadowCBuffer);

	size_t	Size = m_vecInstancingPool.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecInstancingPool[i]);
	}

	m_vecInstancingPool.clear();

	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE((*iter));
	}

	SAFE_DELETE(m_RenderStateManager);
}

void CRenderManager::SetShaderType(EShaderType Type)
{
	m_ShaderType = Type;

	std::string	Name;

	switch (m_ShaderType)
	{
	case EShaderType::Default:
		Name = "LightAccShader";
		break;
	case EShaderType::CelShader:
		Name = "LightCelShader";
		break;
	}

	CSceneManager::GetInst()->GetScene()->GetLightManager()->SetLightAccShader(Name);
}

CRenderInstancing* CRenderManager::FindInstancing(CMesh* Mesh)
{
	auto	iter = m_mapInstancing.find(Mesh);

	if (iter == m_mapInstancing.end())
		return nullptr;

	return iter->second;
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;

	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}

	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerAlphaBlend(const std::string& Name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->AlphaBlend = FindRenderState<CRenderState>("AlphaBlend");
			break;
		}
	}
}

void CRenderManager::DeleteLayer(const std::string& Name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->Name == Name)
		{
			SAFE_DELETE((*iter));
			iter = m_RenderLayerList.erase(iter);
			break;
		}
	}
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->Name == Component->GetRenderLayerName())
		{
			(*iter)->RenderList.push_back(Component);
			break;
		}
	}
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	if (!m_RenderStateManager->Init())
		return false;

	CreateLayer("Default", 1);
	CreateLayer("Back", 0);
	CreateLayer("Particle", 3);
	CreateLayer("Decal", 2);

	SetLayerAlphaBlend("Default");

	m_AlphaBlend = m_RenderStateManager->FindRenderState<CBlendState>("AlphaBlend");
	m_MRTAlphaBlend = m_RenderStateManager->FindRenderState<CBlendState>("MRTAlphaBlend");
	m_DepthDisable = m_RenderStateManager->FindRenderState<CDepthStencilState>("DepthDisable");
	m_DepthWriteDisable = m_RenderStateManager->FindRenderState<CDepthStencilState>("DepthWriteDisable");
	m_LightAccBlend = m_RenderStateManager->FindRenderState<CBlendState>("LightAccBlend");

	return true;
}

void CRenderManager::Render(float DeltaTime)
{
	if (CEngine::GetInst()->GetRender2D())
	{
		{
			auto	iter = m_RenderLayerList.begin();
			auto	iterEnd = m_RenderLayerList.end();

			for (; iter != iterEnd; iter++)
			{
				auto	iter1 = (*iter)->RenderList.begin();
				auto	iter1End = (*iter)->RenderList.end();

				if ((*iter)->AlphaBlend)
					(*iter)->AlphaBlend->SetState();

				for (; iter1 != iter1End;)
				{
					if (!(*iter1)->GetActive())
					{
						iter1 = (*iter)->RenderList.erase(iter1);
						iter1End = (*iter)->RenderList.end();
						continue;
					}

					else if (!(*iter1)->GetEnable())
					{
						iter1++;
						continue;
					}

					(*iter1)->Render();
					iter1++;
				}

				if ((*iter)->AlphaBlend)
					(*iter)->AlphaBlend->ResetState();
			}
		}
	}

	else
	{
		Render3D(DeltaTime);
	}

	



	// 2D, 3D 물체를 모두 출력했다면 UI를 출력해준다.
	// 깊이버퍼를 안쓰고 알파블렌드를 적용한다.
	m_AlphaBlend->SetState();
	m_DepthDisable->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	m_DepthDisable->ResetState();
	m_AlphaBlend->ResetState();

#ifdef _DEBUG

	m_AlphaBlend->SetState();
	m_DepthDisable->SetState();

	CResourceManager::GetInst()->RenderTexture();

	m_DepthDisable->ResetState();
	m_AlphaBlend->ResetState();

#endif // _DEBUG
}

void CRenderManager::Render3D(float DeltaTime)
{
	// 인스턴싱으로 처리해야될 물체가 있을 경우 별도의 인스턴싱 리스트에
	// 추가해주도록 한다.

	// 하늘을 그린다.
	CGameObject* SkyObj = CSceneManager::GetInst()->GetScene()->GetSky();

	if (SkyObj)
	{
		SkyObj->GetRootComponent()->Render();
	}

	// ShadowMap 을 그려낸다.
	RenderShadowMap(DeltaTime);

	// GBuffer를 그려낸다.
	RenderGBuffer(DeltaTime);

	// Decal을 그려낸다.
	RenderDecal(DeltaTime);

	// 조명 처리된 정보를 그려낸다.
	RenderLight(DeltaTime);

	// GBuffer와 Light를 합한 최종 화면을 만든다.
	RenderScreen(DeltaTime);

	// 완성된 타겟을 백버퍼에 출력한다.
	RenderDeferred(DeltaTime);

	// 파티클 출력
	RenderParticle(DeltaTime);
}

void CRenderManager::RenderShadowMap(float DeltaTime)
{
	D3D11_VIEWPORT	vp = {};
	D3D11_VIEWPORT	PrevVp = {};

	UINT Count = 1;
	CDevice::GetInst()->GetContext()->RSGetViewports(&Count, &PrevVp);

	vp.Width = (float)m_ShadowMapRS.Width;
	vp.Height = (float)m_ShadowMapRS.Height;
	vp.MaxDepth = 1.f;

	CDevice::GetInst()->GetContext()->RSSetViewports(1, &vp);

	m_ShadowMapTarget->ClearTarget();

	m_ShadowMapTarget->SetTarget();

	m_NormalRenderList.clear();


	RenderLayer* GBufferLayer = FindLayer("Default");

	auto iter = GBufferLayer->RenderList.begin();
	auto iterEnd = GBufferLayer->RenderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = GBufferLayer->RenderList.erase(iter);
			iterEnd = GBufferLayer->RenderList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		else if ((*iter)->GetFrustumCull())
		{
			iter++;
			continue;
		}

		// 인스턴싱이 되는 물체들을 판단한다.
		if ((*iter)->GetSceneComponentType() == SceneComponentType::Primitive)
		{
			CMesh* Mesh = ((CPrimitiveComponent*)iter->Get())->GetMesh();

			if (Mesh->GetRenderCount() >= 5)
			{
				CRenderInstancing* Instancing = nullptr;

				Instancing = FindInstancing(Mesh);

				// Pool에서 얻어온다.
				if (!Instancing)
				{
					if (m_EmptyPoolList.empty())
					{
						std::vector<CRenderInstancing*>	NewPool = m_vecInstancingPool;

						m_vecInstancingPool.clear();

						m_vecInstancingPool.resize(NewPool.size() * 2);

						for (size_t i = 0; i < NewPool.size(); i++)
						{
							m_vecInstancingPool[i] = NewPool[i];
						}

						for (size_t i = NewPool.size(); i < NewPool.size() * 2; i++)
						{
							m_vecInstancingPool[i] = new CRenderInstancing;
							m_vecInstancingPool[i]->m_PoolIndex = (int)i;

							m_EmptyPoolList.push_back((int)i);
						}
					}

					int Index = m_EmptyPoolList.front();

					m_EmptyPoolList.pop_front();

					m_mapInstancing.insert(std::make_pair(Mesh, m_vecInstancingPool[Index]));

					m_vecInstancingPool[Index]->m_Key = Mesh;

					Instancing = m_vecInstancingPool[Index];
				}

				Instancing->m_LayerName = "Default";
				Instancing->AddRenderList((CPrimitiveComponent*)iter->Get());
			}

			else
			{
				m_NormalRenderList.push_back(*iter);
			}
		}

		else
		{
			m_NormalRenderList.push_back(*iter);
		}

		//(*iter)->Render();
		iter++;
	}

	// 인스턴싱이 아닌 물체를 그려낸다.
	auto iter1 = m_NormalRenderList.begin();
	auto iter1End = m_NormalRenderList.end();

	for (; iter1 != iter1End; iter1++)
	{
		(*iter1)->RenderShadowMap();
	}

	auto iter2 = m_mapInstancing.begin();
	auto iter2End = m_mapInstancing.end();

	for (; iter2 != iter2End;)
	{
		if (iter2->second->GetInstancingCount() == 0)
		{
			m_EmptyPoolList.push_back(iter2->second->GetPoolIndex());
			iter2->second->m_Key = nullptr;

			iter2 = m_mapInstancing.erase(iter2);
			iter2End = m_mapInstancing.end();

			continue;
		}

		iter2++;
	}

	iter2 = m_mapInstancing.begin();
	iter2End = m_mapInstancing.end();

	for (; iter2 != iter2End; iter2++)
	{
		if (iter2->second->m_LayerName != "Default")
		{
			continue;
		}

		iter2->second->RenderShadowMap();
	}

	m_ShadowMapTarget->ResetTarget();

	CDevice::GetInst()->GetContext()->RSSetViewports(1, &PrevVp);
}

void CRenderManager::RenderGBuffer(float DeltaTime)
{
	// GBuffer Target을 지워준다.
	size_t	Size = m_vecGBuffer.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecGBuffer[i]->ClearTarget();
	}

	// GBuffer Target을 지정한다.
	std::vector<ID3D11RenderTargetView*>	vecTargetView;
	std::vector<ID3D11RenderTargetView*>	vecPrevTargetView;

	vecPrevTargetView.resize(Size);

	for (size_t i = 0; i < Size; i++)
	{
		vecTargetView.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	ID3D11DepthStencilView* DepthView = nullptr;

	CDevice::GetInst()->GetContext()->OMGetRenderTargets((UINT)Size, &vecPrevTargetView[0], &DepthView);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)Size, &vecTargetView[0], DepthView);

	// GBuffer에 그릴 내용을 출력한다.
	RenderLayer* GBufferLayer = FindLayer("Default");

	//
	//std::list<CSceneComponent*>	RenderList;
	//
	//auto	iter = GBufferLayer->RenderList.begin();
	//auto	iterEnd = GBufferLayer->RenderList.end();

	//for (; iter != iterEnd;)
	//{
	//	if (!(*iter)->GetActive())
	//	{
	//		iter = GBufferLayer->RenderList.erase(iter);
	//		iterEnd = GBufferLayer->RenderList.end();
	//		continue;
	//	}

	//	else if (!(*iter)->GetEnable())
	//	{
	//		++iter;
	//		continue;
	//	}

	//	else if ((*iter)->GetFrustumCull())
	//	{
	//		++iter;
	//		continue;
	//	}

	//	// 인스턴싱이 되는 물체들을 판단한다.
	//	if ((*iter)->GetSceneComponentType() ==
	//		SceneComponentType::Primitive)
	//	{
	//		CMesh* Mesh = ((CPrimitiveComponent*)iter->Get())->GetMesh();

	//		if (Mesh->GetRenderCount() >= 5)
	//		{
	//			CRenderInstancing* Instancing = nullptr;

	//			Instancing = FindInstancing(Mesh);

	//			// Pool에서 얻어온다.
	//			if (!Instancing)
	//			{
	//				if (m_EmptyPoolList.empty())
	//				{
	//					std::vector<CRenderInstancing*>	NewPool =
	//						m_vecInstancingPool;

	//					m_vecInstancingPool.clear();

	//					m_vecInstancingPool.resize(NewPool.size() * 2);

	//					for (size_t i = 0; i < NewPool.size(); ++i)
	//					{
	//						m_vecInstancingPool[i] = NewPool[i];
	//					}

	//					for (size_t i = NewPool.size(); i < NewPool.size() * 2; ++i)
	//					{
	//						m_vecInstancingPool[i] = new CRenderInstancing;
	//						m_vecInstancingPool[i]->m_PoolIndex = (int)i;

	//						m_EmptyPoolList.push_back((int)i);
	//					}
	//				}

	//				int Index = m_EmptyPoolList.front();

	//				m_EmptyPoolList.pop_front();

	//				m_mapInstancing.insert(std::make_pair(Mesh, m_vecInstancingPool[Index]));

	//				m_vecInstancingPool[Index]->m_Key = Mesh;

	//				Instancing = m_vecInstancingPool[Index];
	//			}

	//			Instancing->m_LayerName = "Default";
	//			Instancing->AddRenderList((CPrimitiveComponent*)iter->Get());
	//		}

	//		else
	//		{
	//			RenderList.push_back(*iter);
	//		}
	//	}

	//	else
	//	{
	//		RenderList.push_back(*iter);
	//	}

	//	//(*iter)->Render();
	//	++iter;
	//}

	// 인스턴싱이 아닌 물체를 그려낸다.
	auto iter1 = m_NormalRenderList.begin();
	auto iter1End = m_NormalRenderList.end();

	for (; iter1 != iter1End; iter1++)
	{
		(*iter1)->Render();
	}

	//auto	iter2 = m_mapInstancing.begin();
	//auto	iter2End = m_mapInstancing.end();

	//for (; iter2 != iter2End;)
	//{
	//	if (iter2->second->GetInstancingCount() == 0)
	//	{
	//		m_EmptyPoolList.push_back(iter2->second->GetPoolIndex());
	//		iter2->second->m_Key = nullptr;

	//		iter2 = m_mapInstancing.erase(iter2);
	//		iter2End = m_mapInstancing.end();

	//		continue;
	//	}

	//	++iter2;
	//}

	auto iter2 = m_mapInstancing.begin();
	auto iter2End = m_mapInstancing.end();

	for (; iter2 != iter2End; iter2++)
	{
		if (iter2->second->m_LayerName != "Default")
		{
			continue;
		}

		iter2->second->Render();
	}

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)Size, &vecPrevTargetView[0], DepthView);

	SAFE_RELEASE(DepthView);

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_RELEASE(vecPrevTargetView[i]);
	}
}

void CRenderManager::RenderDecal(float DeltaTime)
{
	// DecalBuffer Target을 지워준다.
	size_t	Size = m_vecDecalBuffer.size();

	// DecalBuffer Target을 지정한다.
	std::vector<ID3D11RenderTargetView*> vecTargetView;
	std::vector<ID3D11RenderTargetView*> vecPrevTargetView;

	vecPrevTargetView.resize(Size);

	for (size_t i = 0; i < Size; i++)
	{
		vecTargetView.push_back(m_vecDecalBuffer[i]->GetTargetView());
	}

	ID3D11DepthStencilView* DepthView = nullptr;

	CDevice::GetInst()->GetContext()->OMGetRenderTargets((UINT)Size, &vecPrevTargetView[0], &DepthView);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)Size, &vecTargetView[0], DepthView);

	m_DepthWriteDisable->SetState();

	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecGBuffer[4]->SetTargetShader(18);
	m_vecGBuffer[5]->SetTargetShader(19);

	m_MRTAlphaBlend->SetState();


	RenderLayer* DecalLayer = FindLayer("Decal");


	std::list<CSceneComponent*>	RenderList;

	auto iter = DecalLayer->RenderList.begin();
	auto iterEnd = DecalLayer->RenderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = DecalLayer->RenderList.erase(iter);
			iterEnd = DecalLayer->RenderList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		else if ((*iter)->GetFrustumCull())
		{
			iter++;
			continue;
		}

		(*iter)->Render();
		iter++;
	}

	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecGBuffer[4]->ResetTargetShader(18);
	m_vecGBuffer[5]->ResetTargetShader(19);

	m_MRTAlphaBlend->ResetState();
	m_DepthWriteDisable->ResetState();


	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)Size, &vecPrevTargetView[0], DepthView);

	SAFE_RELEASE(DepthView);

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_RELEASE(vecPrevTargetView[i]);
	}
}

void CRenderManager::RenderLight(float DeltaTime)
{
	// Light Target을 지워준다.
	size_t	Size = m_vecLightBuffer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecLightBuffer[i]->ClearTarget();
	}

	// Light Target을 지정한다.
	std::vector<ID3D11RenderTargetView*>	vecTargetView;
	std::vector<ID3D11RenderTargetView*>	vecPrevTargetView;

	vecPrevTargetView.resize(Size);

	for (size_t i = 0; i < Size; ++i)
	{
		vecTargetView.push_back(m_vecLightBuffer[i]->GetTargetView());
	}

	ID3D11DepthStencilView* DepthView = nullptr;

	CDevice::GetInst()->GetContext()->OMGetRenderTargets((UINT)Size,
		&vecPrevTargetView[0], &DepthView);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)Size,
		&vecTargetView[0], DepthView);

	m_DepthDisable->SetState();
	m_LightAccBlend->SetState();

	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[1]->SetTargetShader(15);
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecGBuffer[3]->SetTargetShader(17);

	CSceneManager::GetInst()->GetScene()->GetLightManager()->Render();

	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[1]->ResetTargetShader(15);
	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecGBuffer[3]->ResetTargetShader(17);

	m_LightAccBlend->ResetState();
	m_DepthDisable->ResetState();



	CDevice::GetInst()->GetContext()->OMSetRenderTargets((UINT)Size,
		&vecPrevTargetView[0], DepthView);

	SAFE_RELEASE(DepthView);

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_RELEASE(vecPrevTargetView[i]);
	}
}

void CRenderManager::RenderScreen(float DeltaTime)
{
	m_ScreenBuffer->ClearTarget();

	m_ScreenBuffer->SetTarget();

	m_ScreenShader->SetShader();

	m_DepthDisable->SetState();

	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[2]->SetTargetShader(16);

	m_vecLightBuffer[0]->SetTargetShader(18);
	m_vecLightBuffer[1]->SetTargetShader(19);
	m_vecLightBuffer[2]->SetTargetShader(20);

	m_ShadowMapTarget->SetTargetShader(22);

	Matrix	matView, matProj;

	matView = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetShadowViewMatrix();
	matProj = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetShadowProjMatrix();

	Matrix	matVP = matView * matProj;

	m_ShadowCBuffer->SetShadowVP(matVP);

	m_ShadowCBuffer->UpdateBuffer();

	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	UINT	Offset = 0;

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Context->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	Context->Draw(4, 0);

	m_DepthDisable->ResetState();

	m_ShadowMapTarget->ResetTargetShader(22);

	m_vecGBuffer[0]->ResetTargetShader(14);

	m_vecLightBuffer[0]->ResetTargetShader(18);
	m_vecLightBuffer[1]->ResetTargetShader(19);
	m_vecLightBuffer[2]->ResetTargetShader(20);

	m_ScreenBuffer->ResetTarget();
}

void CRenderManager::RenderDeferred(float DeltaTime)
{
	m_DeferredRenderShader->SetShader();

	m_DepthDisable->SetState();

	m_ScreenBuffer->SetTargetShader(21);

	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	UINT	Offset = 0;

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Context->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	Context->Draw(4, 0);


	m_DepthDisable->ResetState();

	m_ScreenBuffer->ResetTargetShader(21);

	// 디버그 모드일 경우 데칼 디버깅용 육면체를 출력한다.
#ifdef _DEBUG

	RenderLayer* DecalLayer = FindLayer("Decal");


	std::list<CSceneComponent*>	RenderList;

	auto	iter = DecalLayer->RenderList.begin();
	auto	iterEnd = DecalLayer->RenderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = DecalLayer->RenderList.erase(iter);
			iterEnd = DecalLayer->RenderList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		((CDecalComponent*)(*iter).Get())->RenderDebug();
		++iter;
	}

#endif // _DEBUG
}

void CRenderManager::RenderParticle(float DeltaTime)
{
	RenderLayer* ParticleLayer = FindLayer("Particle");

	std::list<CSceneComponent*>	RenderList;

	m_AlphaBlend->SetState();

	m_vecGBuffer[2]->SetTargetShader(14);

	ParticleLayer->RenderList.sort(CRenderManager::SortAlphaObject);

	auto iter = ParticleLayer->RenderList.begin();
	auto iterEnd = ParticleLayer->RenderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = ParticleLayer->RenderList.erase(iter);
			iterEnd = ParticleLayer->RenderList.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		else if ((*iter)->GetFrustumCull())
		{
			iter++;
			continue;
		}

		// 인스턴싱이 되는 물체들을 판단한다.
		if ((*iter)->GetSceneComponentType() == SceneComponentType::Primitive && !(*iter)->CheckTypeID<CParticleComponent>())
		{
			CMesh* Mesh = ((CPrimitiveComponent*)iter->Get())->GetMesh();

			if (Mesh->GetRenderCount() >= 5)
			{
				CRenderInstancing* Instancing = nullptr;

				Instancing = FindInstancing(Mesh);

				// Pool에서 얻어온다.
				if (!Instancing)
				{
					if (m_EmptyPoolList.empty())
					{
						std::vector<CRenderInstancing*>	NewPool = m_vecInstancingPool;

						m_vecInstancingPool.clear();

						m_vecInstancingPool.resize(NewPool.size() * 2);

						for (size_t i = 0; i < NewPool.size(); i++)
						{
							m_vecInstancingPool[i] = NewPool[i];
						}

						for (size_t i = NewPool.size(); i < NewPool.size() * 2; i++)
						{
							m_vecInstancingPool[i] = new CRenderInstancing;
							m_vecInstancingPool[i]->m_PoolIndex = (int)i;

							m_EmptyPoolList.push_back((int)i);
						}
					}

					int Index = m_EmptyPoolList.front();

					m_EmptyPoolList.pop_front();

					m_mapInstancing.insert(std::make_pair(Mesh, m_vecInstancingPool[Index]));

					m_vecInstancingPool[Index]->m_Key = Mesh;

					Instancing = m_vecInstancingPool[Index];
				}

				Instancing->m_LayerName = "Particle";
				Instancing->AddRenderList((CPrimitiveComponent*)iter->Get());
			}

			else
			{
				RenderList.push_back(*iter);
			}
		}

		else
		{
			RenderList.push_back(*iter);
		}

		//(*iter)->Render();
		iter++;
	}

	// 인스턴싱이 아닌 물체를 그려낸다.
	auto	iter1 = RenderList.begin();
	auto	iter1End = RenderList.end();

	for (; iter1 != iter1End; iter1++)
	{
		(*iter1)->Render();
	}

	auto iter2 = m_mapInstancing.begin();
	auto iter2End = m_mapInstancing.end();

	for (; iter2 != iter2End;)
	{
		if (iter2->second->GetInstancingCount() == 0)
		{
			m_EmptyPoolList.push_back(iter2->second->GetPoolIndex());
			iter2->second->m_Key = nullptr;

			iter2 = m_mapInstancing.erase(iter2);
			iter2End = m_mapInstancing.end();

			continue;
		}

		iter2++;
	}

	iter2 = m_mapInstancing.begin();
	iter2End = m_mapInstancing.end();

	for (; iter2 != iter2End; iter2++)
	{
		if (iter2->second->m_LayerName != "Particle")
		{
			iter2++;
			continue;
		}

		iter2->second->Render();
	}

	m_AlphaBlend->ResetState();

	m_vecGBuffer[2]->ResetTargetShader(14);
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g, float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable, 
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp,
	D3D11_BLEND SrcAlphBlend, D3D11_BLEND DestAlphBlend, D3D11_BLEND_OP BlendAlphOp,
	UINT8 WriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend, BlendOp, SrcAlphBlend, DestAlphBlend, BlendAlphOp, WriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

bool CRenderManager::CreateDepthStencil(const std::string& Name, 
	bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, 
	D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable, 
	UINT8 StencilReadMask, UINT8 StencilWriteMask, 
	D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	return m_RenderStateManager->CreateDepthStencil(Name, DepthEnable,
		DepthWriteMask, DepthFunc, StencilEnable, StencilReadMask, StencilWriteMask,
		FrontFace, BackFace);
}

bool CRenderManager::SortLayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}

void CRenderManager::CreateRenderTarget()
{
	// 인스턴싱용 메모리 풀 생성
	m_vecInstancingPool.resize(30);

	size_t	Size = m_vecInstancingPool.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecInstancingPool[i] = new CRenderInstancing;

		m_vecInstancingPool[i]->m_PoolIndex = (int)i;
	}

	for (int i = 0; i < 30; ++i)
	{
		m_EmptyPoolList.push_back(i);
	}

	Resolution	RS = CDevice::GetInst()->GetResolution();

	CResourceManager::GetInst()->CreateTarget("GBuffer1", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CResourceManager::GetInst()->CreateTarget("GBuffer2", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CResourceManager::GetInst()->CreateTarget("GBuffer3", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CResourceManager::GetInst()->CreateTarget("GBuffer4", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CResourceManager::GetInst()->CreateTarget("GBuffer5", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CResourceManager::GetInst()->CreateTarget("GBuffer6", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CRenderTarget* GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");

	GBufferTarget->SetPos(Vector3(0.f, 0.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);
	m_vecDecalBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer2");

	GBufferTarget->SetPos(Vector3(0.f, 100.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);
	m_vecDecalBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");

	GBufferTarget->SetPos(Vector3(0.f, 200.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);
	m_vecDecalBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer4");

	GBufferTarget->SetPos(Vector3(0.f, 300.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);
	m_vecDecalBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer5");

	GBufferTarget->SetPos(Vector3(0.f, 400.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer6");

	GBufferTarget->SetPos(Vector3(0.f, 500.f, 0.f));
	GBufferTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);


	// Light Target
	CResourceManager::GetInst()->CreateTarget("LightDiffuse", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CResourceManager::GetInst()->CreateTarget("LightSpecular", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CResourceManager::GetInst()->CreateTarget("LightEmissive", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	CRenderTarget* LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightDiffuse");

	LightTarget->SetPos(Vector3(100.f, 0.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightSpecular");

	LightTarget->SetPos(Vector3(100.f, 100.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	LightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightEmissive");

	LightTarget->SetPos(Vector3(100.f, 200.f, 0.f));
	LightTarget->SetScale(Vector3(100.f, 100.f, 1.f));
	LightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(LightTarget);

	// RenderScreenTarget
	CResourceManager::GetInst()->CreateTarget("RenderScreen", RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_ScreenBuffer = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("RenderScreen");

	m_ScreenBuffer->SetPos(Vector3(200.f, 0.f, 0.f));
	m_ScreenBuffer->SetScale(Vector3(100.f, 100.f, 1.f));
	m_ScreenBuffer->SetDebugRender(true);

	// ShadowMapTarget
	m_ShadowMapRS.Width = 2560;
	m_ShadowMapRS.Height = 1440;
	CResourceManager::GetInst()->CreateTarget("ShadowMap", m_ShadowMapRS.Width, m_ShadowMapRS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D24_UNORM_S8_UINT);

	m_ShadowMapTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("ShadowMap");

	m_ShadowMapTarget->SetPos(Vector3(300.f, 100.f, 0.f));
	m_ShadowMapTarget->SetScale(Vector3(300.f, 300.f, 1.f));
	m_ShadowMapTarget->SetDebugRender(true);

	m_ShadowCBuffer = new CShadowConstantBuffer;

	m_ShadowCBuffer->Init();

	m_ShadowCBuffer->SetShadowResolution(Vector2((float)m_ShadowMapRS.Width, (float)m_ShadowMapRS.Height));

	m_ScreenShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("ScreenShader");
	m_DeferredRenderShader = (CGraphicShader*)CResourceManager::GetInst()->FindShader("DeferredRenderShader");
}

bool CRenderManager::SortAlphaObject(CSceneComponent* Src, CSceneComponent* Dest)
{
	// 뷰공간의 z값을 비교한다.
	Vector3	SrcPos = Src->GetWorldPos();
	Vector3	DestPos = Dest->GetWorldPos();

	Matrix ViewMatrix = Src->GetScene()->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();

	SrcPos = SrcPos.TransformCoord(ViewMatrix);
	DestPos = DestPos.TransformCoord(ViewMatrix);

	return SrcPos.z < DestPos.z;
}

RenderLayer* CRenderManager::FindLayer(const std::string& Name)
{
	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	RenderLayer* GBufferLayer = nullptr;

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->Name == Name)
			return *iter;
	}

	return nullptr;
}
