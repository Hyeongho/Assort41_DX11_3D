#pragma once

#include "../../EngineInfo.h"
#include "Material.h"

class CMaterialManager
{
	friend class CResourceManager;

private:
	CMaterialManager();
	~CMaterialManager();

private:
	std::unordered_map<std::string, CSharedPtr<CMaterial>>	m_mapMaterial;

public:
	//김범중 이름 가져오는 함수
	void GetMaterialNames(std::vector<std::string>& vecNames)
	{
		auto	iter = m_mapMaterial.begin();
		auto	iterEnd = m_mapMaterial.end();
		for (; iter != iterEnd; ++iter)
		{
			vecNames.push_back(iter->first);
		}
	}

	bool Init();
	CMaterial* FindMaterial(const std::string& Name);
	void ReleaseMaterial(const std::string& Name);

public:
	template <typename T>
	T* CreateMaterial(const std::string& Name)
	{
		T* Material = (T*)FindMaterial(Name);

		if (Material)
		{
			return Material;
		}

		Material = new T;

		Material->SetName(Name);

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return Material;
	}
};

