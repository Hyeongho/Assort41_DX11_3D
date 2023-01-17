#pragma once

#include "../../EngineInfo.h"

class CShaderManager
{
	friend class CResourceManager;

private:
	CShaderManager();
	~CShaderManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CShader>>	m_mapShader;
	std::unordered_map<std::string, CSharedPtr<class CShader>>	m_mapGlobalShader;
	std::unordered_map<std::string, CSharedPtr<class CConstantBuffer>>	m_mapCBuffer;
	class CColliderConstantBuffer* m_ColliderCBuffer;

public:
	class CColliderConstantBuffer* GetColliderCBuffer()	const
	{
		return m_ColliderCBuffer;
	}

public:
	bool Init();
	class CShader* FindShader(const std::string& Name);
	void ReleaseShader(const std::string& Name);

	bool CreateConstantBuffer(const std::string& Name, int Size, int Register,
		int ShaderBufferType = (int)EShaderBufferType::All);
	class CConstantBuffer* FindConstantBuffer(const std::string& Name);

public:
	template <typename T>
	bool CreateShader(const std::string& Name, bool GlobalShader = false)
	{
		T* Shader = (T*)FindShader(Name);

		if (Shader)
			return true;

		Shader = new T;

		Shader->SetName(Name);

		if (!Shader->Init())
		{
			SAFE_DELETE(Shader);
			return false;
		}

		m_mapShader.insert(std::make_pair(Name, Shader));

		if (GlobalShader)
			m_mapGlobalShader.insert(std::make_pair(Name, Shader));

		return true;
	}
};

