#include "Particle.h"
#include "../Shader/StructuredBuffer.h"
#include "../Shader/ParticleConstantBuffer.h"
#include "../Shader/ParticleUpdateShader.h"
#include "../Shader/ParticleRenderShader.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CParticle::CParticle() :
	m_Scene(nullptr),
	m_CBuffer(nullptr)
{
}

CParticle::~CParticle()
{
	size_t	Size = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);
}

bool CParticle::Init()
{
	m_CBuffer = new CParticleConstantBuffer;

	m_CBuffer->Init();

	SetUpdateShader("ParticleUpdateShader");
	SetRenderShader("ParticleRenderShader");

	AddStructuredBuffer("ParticleInfo", sizeof(ParticleInfo), m_CBuffer->m_BufferData.ParticleSpawnCountMax, 0);

	AddStructuredBuffer("ParticleInfoShare", sizeof(ParticleInfoShare), 1, 1);


	return true;
}

void CParticle::SetUpdateShader(const std::string& Name)
{
	if (m_Scene)
	{
		m_UpdateShader = (CParticleUpdateShader*)m_Scene->GetResource()->FindShader(Name);
	}

	else
	{
		m_UpdateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindShader(Name);
	}
}

void CParticle::SetRenderShader(const std::string& Name)
{
	if (m_Scene)
	{
		m_RenderShader = (CParticleRenderShader*)m_Scene->GetResource()->FindShader(Name);
	}

	else
	{
		m_RenderShader = (CParticleRenderShader*)CResourceManager::GetInst()->FindShader(Name);
	}
}

void CParticle::AddStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count, int Register,
	bool Dynamic, int StructuredBufferShaderType)
{
	CStructuredBuffer* Buffer = new CStructuredBuffer;

	if (!Buffer->Init(Name, Size, Count, Register, Dynamic, StructuredBufferShaderType))
	{
		SAFE_DELETE(Buffer);
		return;
	}

	m_vecStructuredBuffer.push_back(Buffer);
}

void CParticle::ResizeStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count, int Register,
	bool Dynamic, int StructuredBufferShaderType)
{
	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		if (m_vecStructuredBuffer[i]->GetName() == Name)
		{
			m_vecStructuredBuffer[i]->Init(Name, Size, Count, Register, Dynamic, StructuredBufferShaderType);
		}
	}
}

void CParticle::CloneStructuredBuffer(
	std::vector<class CStructuredBuffer*>& vecBuffer)
{
	size_t	Size = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < Size; ++i)
	{
		vecBuffer.push_back(m_vecStructuredBuffer[i]->Clone());
	}
}

void CParticle::SetParticleSpawnCountMax(int Count)
{
	m_CBuffer->SetParticleSpawnCountMax(Count);
}

void CParticle::SetParticleSpawnTime(float Time)
{
	m_SpawnTime = Time;
}

void CParticle::SetMaterial(const std::string& Name)
{
	if (m_Scene)
	{
		m_Material = (CMaterial*)m_Scene->GetResource()->FindMaterial(Name);
	}

	else
	{
		m_Material = (CMaterial*)CResourceManager::GetInst()->FindMaterial(Name);
	}
}

CParticleConstantBuffer* CParticle::CloneCBuffer() const
{
	return m_CBuffer->Clone();
}

void CParticle::SetMaterial(CMaterial* Material)
{
	m_Material = Material;
}

void CParticle::SetParticleEnable(bool Enable)
{
	m_CBuffer->SetParticleEnable(Enable);
}

void CParticle::SetParticleStartMin(const Vector3& Min)
{
	m_CBuffer->SetParticleStartMin(Min);
}

void CParticle::SetParticleStartMax(const Vector3& Max)
{
	m_CBuffer->SetParticleStartMax(Max);
}

void CParticle::SetParticleScaleMin(const Vector3& Min)
{
	m_CBuffer->SetParticleScaleMin(Min);
}

void CParticle::SetParticleScaleMax(const Vector3& Max)
{
	m_CBuffer->SetParticleScaleMax(Max);
}

void CParticle::SetParticleLifeTimeMin(float Min)
{
	m_CBuffer->SetParticleLifeTimeMin(Min);
}

void CParticle::SetParticleLifeTimeMax(float Max)
{
	m_CBuffer->SetParticleLifeTimeMax(Max);
}

void CParticle::SetParticleColorMin(const Vector4& Min)
{
	m_CBuffer->SetParticleColorMin(Min);
}

void CParticle::SetParticleColorMax(const Vector4& Max)
{
	m_CBuffer->SetParticleColorMax(Max);
}

void CParticle::SetParticleSpeedMin(float Min)
{
	m_CBuffer->SetParticleSpeedMin(Min);
}

void CParticle::SetParticleSpeedMax(float Max)
{
	m_CBuffer->SetParticleSpeedMax(Max);
}

void CParticle::SetParticleMoveEnable(bool Enable)
{
	m_CBuffer->SetParticleMoveEnable(Enable);
}

void CParticle::SetParticleGravityEnable(bool Enable)
{
	m_CBuffer->SetParticleGravityEnable(Enable);
}

void CParticle::SetParticleMoveDir(const Vector3& MoveDir)
{
	m_CBuffer->SetParticleMoveDir(MoveDir);
}

void CParticle::SetParticleMoveDirEnable(bool Enable)
{
	m_CBuffer->SetParticleMoveDirEnable(Enable);
}

void CParticle::SetParticleMoveAngle(const Vector3& MoveAngle)
{
	m_CBuffer->SetParticleMoveAngle(MoveAngle);
}
