#include "RenderInstancing.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../Component/CameraComponent.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh/AnimationMesh.h"

CRenderInstancing::CRenderInstancing()	:
	m_PoolIndex(-1),
	m_Key(nullptr)
{
	m_BufferCount = 100;
	m_Buffer = new CStructuredBuffer;

	m_Buffer->Init("InstancingBuffer", sizeof(InstancingBufferInfo),
		m_BufferCount, 50, true, (int)EShaderBufferType::All);

	m_InstancingShader = CResourceManager::GetInst()->FindShader("MeshInstancingShader");

	m_CBuffer = new CInstancingConstantBuffer;

	m_CBuffer->Init();
}

CRenderInstancing::~CRenderInstancing()
{
	SAFE_DELETE(m_CBuffer);
	SAFE_DELETE(m_Buffer);
}

void CRenderInstancing::Render()
{
	if(m_RenderList.empty())
	{
		return;
	}
	auto	iter = m_RenderList.begin();
	auto	iterEnd = m_RenderList.end();

	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Matrix	matView, matProj;
	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

	std::vector<InstancingBufferInfo>	vecBuffer;

	vecBuffer.resize(m_RenderList.size());

	for (int i = 0; iter != iterEnd; iter++, ++i)
	{
		Matrix	matWorld = (*iter)->GetWorldMatrix();

		Matrix	matResult = matWorld * matView;

		// 데이터를 모은다.
		vecBuffer[i].matWV = matResult;
		vecBuffer[i].matWVP = matResult * matProj;

		vecBuffer[i].matWV.Transpose();
		vecBuffer[i].matWVP.Transpose();
	}

	if (m_Key->GetMeshType() == MeshType::Animation)
	{
		((CAnimationMesh*)m_Key)->SetBoneShader();
		m_CBuffer->SetBoneCount(((CAnimationMesh*)m_Key)->GetBoneCount());
	}

	m_CBuffer->UpdateBuffer();

	CMaterial* RenderMaterial = m_RenderList.front()->GetMaterial(0);

	int	SlotCount = m_RenderList.front()->GetSlotCount();

	for (int Slot = 0; Slot < SlotCount; ++Slot)
	{
		iter = m_RenderList.begin();
		iterEnd = m_RenderList.end();

		for (int i = 0; iter != iterEnd; iter++, ++i)
		{
			CMaterial* Material = (*iter)->GetMaterial(Slot);

			Material->CopyInstancingData(vecBuffer[i]);
		}

		m_Buffer->UpdateBuffer(&vecBuffer[0], (int)vecBuffer.size());

		m_Buffer->SetShader();

		RenderMaterial->SetInstancingMaterial();

		m_InstancingShader->SetShader();

		m_Key->RenderInstancing((int)vecBuffer.size(), Slot);

		RenderMaterial->ResetInstancingMaterial();

		m_Buffer->ResetShader();
	}

	if (m_Key->GetMeshType() == MeshType::Animation)
	{
		((CAnimationMesh*)m_Key)->ResetBoneShader();
	}

	m_RenderList.clear();
}

void CRenderInstancing::RenderShadowMap()
{
	auto iter = m_RenderList.begin();
	auto iterEnd = m_RenderList.end();

	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Matrix	matView, matProj;
	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

	std::vector<InstancingBufferInfo>	vecBuffer;

	vecBuffer.resize(m_RenderList.size());

	for (int i = 0; iter != iterEnd; iter++, i++)
	{
		Matrix	matWorld = (*iter)->GetWorldMatrix();

		Matrix	matResult = matWorld * matView;

		// 데이터를 모은다.
		vecBuffer[i].matWV = matResult;
		vecBuffer[i].matWVP = matResult * matProj;

		vecBuffer[i].matWV.Transpose();
		vecBuffer[i].matWVP.Transpose();
	}

	if (m_Key->GetMeshType() == MeshType::Animation)
	{
		((CAnimationMesh*)m_Key)->SetBoneShader();
		m_CBuffer->SetBoneCount(((CAnimationMesh*)m_Key)->GetBoneCount());
	}

	m_CBuffer->UpdateBuffer();

	CMaterial* RenderMaterial = m_RenderList.front()->GetMaterial(0);

	int	SlotCount = m_RenderList.front()->GetSlotCount();

	for (int Slot = 0; Slot < SlotCount; Slot++)
	{
		iter = m_RenderList.begin();
		iterEnd = m_RenderList.end();

		for (int i = 0; iter != iterEnd; iter++, i++)
		{
			CMaterial* Material = (*iter)->GetMaterial(Slot);

			Material->CopyInstancingData(vecBuffer[i]);
		}

		m_Buffer->UpdateBuffer(&vecBuffer[0], (int)vecBuffer.size());

		m_Buffer->SetShader();

		RenderMaterial->SetInstancingMaterial();

		m_InstancingShader->SetShader();

		m_Key->RenderInstancing((int)vecBuffer.size(), Slot);

		RenderMaterial->ResetInstancingMaterial();

		m_Buffer->ResetShader();
	}

	if (m_Key->GetMeshType() == MeshType::Animation)
	{
		((CAnimationMesh*)m_Key)->ResetBoneShader();
	}

	m_RenderList.clear();
}
