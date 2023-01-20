#include "LightManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/LightComponent.h"
#include "Scene.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/Shader.h"
#include "../Device.h"

CLightManager::CLightManager()	:
	m_LightInfoBuffer(nullptr),
	m_LightInfoBufferCount(10)
{
}

CLightManager::~CLightManager()
{
	SAFE_DELETE(m_LightInfoBuffer);
}

void CLightManager::AddLight(CLightComponent* Light)
{
	m_LightList.push_back(Light);

	if (m_LightInfoBufferCount < m_LightList.size())
	{
		m_LightInfoBufferCount *= 2;

		m_LightInfoBuffer->Init("LightInfo", sizeof(LightCBuffer),
			(unsigned int)m_LightInfoBufferCount, 41, true,
			(int)EShaderBufferType::Pixel);
	}
}

void CLightManager::DeleteLight(CLightComponent* Light)
{
	m_LightList.remove(Light);
}

void CLightManager::DeleteLight(const std::string& Name)
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetName() == Name)
		{
			m_LightList.erase(iter);
			break;
		}
	}
}

void CLightManager::Start()
{
}

bool CLightManager::Init()
{
	m_GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");

	m_GlobalLightComponent = m_GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");

	m_GlobalLightComponent->SetLightType(ELightType::Direction);
	m_GlobalLightComponent->SetRelativeRotation(45.f, 90.f, 0.f);

	m_LightInfoBuffer = new CStructuredBuffer;

	m_LightInfoBuffer->Init("LightInfo", sizeof(LightCBuffer),
		(unsigned int)m_LightInfoBufferCount, 41, true,
		(int)EShaderBufferType::Pixel);

	m_LightAccShader = CResourceManager::GetInst()->FindShader("LightAccShader");




	CGameObject* LightObj = m_Owner->CreateObject<CGameObject>("Light1");

	CLightComponent* LightCom = LightObj->CreateComponent<CLightComponent>("Light");

	LightCom->SetLightType(ELightType::Point);
	LightCom->SetWorldPosition(-300.f, 400.f, -100.f);
	LightCom->SetLightColor(Vector4(1.f, 0.f, 0.f, 1.f));
	LightCom->SetLightDistance(2000.f);


	LightObj = m_Owner->CreateObject<CGameObject>("Light2");

	LightCom = LightObj->CreateComponent<CLightComponent>("Light");

	LightCom->SetLightType(ELightType::Point);
	LightCom->SetWorldPosition(300.f, 400.f, -100.f);
	LightCom->SetLightColor(Vector4(0.f, 1.f, 0.f, 1.f));
	LightCom->SetLightDistance(2000.f);



	LightObj = m_Owner->CreateObject<CGameObject>("Light3");

	LightCom = LightObj->CreateComponent<CLightComponent>("Light");

	LightCom->SetLightType(ELightType::Point);
	LightCom->SetWorldPosition(0.f, 100.f, -50.f);
	LightCom->SetLightColor(Vector4(1.f, 0.f, 1.f, 1.f));
	LightCom->SetLightDistance(2000.f);


	return true;
}

void CLightManager::Update(float DeltaTime)
{
	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_LightList.erase(iter);
			iterEnd = m_LightList.end();
			continue;
		}

		iter++;
	}
}

void CLightManager::Render()
{
	if (m_LightList.empty())
		return;

	std::vector<LightCBuffer>	vecInstancingBuffer;

	vecInstancingBuffer.resize(m_LightList.size());

	auto	iter = m_LightList.begin();
	auto	iterEnd = m_LightList.end();

	for (int i = 0; iter != iterEnd; iter++, ++i)
	{
		const LightCBuffer& Info = (*iter)->GetLightInfo();

		vecInstancingBuffer[i] = Info;
	}

	m_LightInfoBuffer->UpdateBuffer(&vecInstancingBuffer[0],
		(int)vecInstancingBuffer.size());


	// 구조화버퍼 적용
	m_LightInfoBuffer->SetShader();


	m_LightAccShader->SetShader();

	iter = m_LightList.begin();

	// Transform 정보를 넘겨준다. World는 의미없고 그 외의 정보들을 넘겨주는게
	// 중요하다.
	(*iter)->GetTransform()->SetTransform();

	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	UINT	Offset = 0;

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Context->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	Context->DrawInstanced(4, (int)vecInstancingBuffer.size(),
		0, 0);
}

void CLightManager::Save(FILE* File)
{
}

void CLightManager::Load(FILE* File)
{
}
