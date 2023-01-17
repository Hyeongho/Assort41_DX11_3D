#pragma once

#include "EngineInfo.h"

struct PathInfo
{
	TCHAR	Path[MAX_PATH];
	char	PathMultibyte[MAX_PATH];

	PathInfo() :
		Path{},
		PathMultibyte{}
	{
	}
};

class CPathManager
{
private:
	std::unordered_map<std::string, PathInfo*>	m_mapPath;

public:
	bool Init();
	bool AddPath(const std::string& Name, const TCHAR* Path,
		const std::string& BasePathName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& Name);

	DECLARE_SINGLE(CPathManager)
};

