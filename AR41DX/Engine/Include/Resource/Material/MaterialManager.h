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
	bool Init();
	CMaterial* FindMaterial(const std::string& Name);
	void ReleaseMaterial(const std::string& Name);

public:
	template <typename T>
	T* CreateMaterial(const std::string& Name)
	{
		T* Material = (T*)FindMaterial(Name);

		if (Material)
			return Material;

		Material = new T;

		Material->SetName(Name);

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return Material;
	}
};

