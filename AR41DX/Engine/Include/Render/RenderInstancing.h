#pragma once

#include "../EngineInfo.h"
#include "../Component/PrimitiveComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Shader/InstancingConstantBuffer.h"

class CRenderInstancing
{
	friend class CRenderManager;

private:
	CRenderInstancing();
	~CRenderInstancing();

private:
	std::list<CPrimitiveComponent*>	m_RenderList;
	std::string	m_LayerName;
	class CMesh* m_Key;
	int m_PoolIndex;
	int m_BufferCount;
	CStructuredBuffer* m_Buffer;
	CSharedPtr<CShader>	m_InstancingShader;
	CInstancingConstantBuffer* m_CBuffer;

public:
	class CMesh* GetKey()	const
	{
		return m_Key;
	}

	int GetPoolIndex()	const
	{
		return m_PoolIndex;
	}

	int GetInstancingCount()	const
	{
		return (int)m_RenderList.size();
	}

public:
	void AddRenderList(CPrimitiveComponent* Component)
	{
		m_RenderList.push_back(Component);

		Component->SetInstanceID((int)(m_RenderList.size() - 1));

		if (m_RenderList.size() > (size_t)m_BufferCount)
		{
			m_BufferCount *= 2;

			m_Buffer->Init("InstancingBuffer", sizeof(InstancingBufferInfo),
				m_BufferCount, 50, true, (int)EShaderBufferType::All);
		}
	}

	void Clear()
	{
		m_RenderList.clear();
	}

public:
	void Render();
};

