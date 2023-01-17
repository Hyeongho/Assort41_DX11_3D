#include "PathManager.h"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	auto	iter = m_mapPath.begin();
	auto	iterEnd = m_mapPath.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	TCHAR	Root[MAX_PATH] = {};

	// �����������ִ� ���������� ��ü���/���������̸�.exe �� ���ڿ���
	// ������ �ȴ�.
	// Bin/aa.exe
	GetModuleFileName(0, Root, MAX_PATH);

	int	Length = lstrlen(Root);

	for (int i = Length - 1; i >= 0; --i)
	{
		if (Root[i] == '/' || Root[i] == '\\')
		{
			memset(&Root[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
			break;
		}
	}

	PathInfo* Info = new PathInfo;

	lstrcpy(Info->Path, Root);

	// #ifdef : �ڿ� �ִ� ������ #define���� ���ǵǾ� �ִ�����
	// �Ǵ��ϴ� if���̴�. 
	// ������ �ܰ迡�� �ڿ� �ִ� ������ #define���� ���ǵǾ� �ִ��� �Ǵ�.
#ifdef UNICODE

	// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ����
	// ���´�.
	int	PathLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		0, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		Info->PathMultibyte, PathLength, 0, 0);

#else

	strcpy_s(Info->PathMultibyte, Info->Path);

#endif // UNICODE


	m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

	AddPath(TEXTURE_PATH, TEXT("Texture/"));
	AddPath(SHADER_PATH, TEXT("Shader/"));
	AddPath(SOUND_PATH, TEXT("Sound/"));
	AddPath(FONT_PATH, TEXT("Font/"));
	AddPath(ANIMATION2D_PATH, TEXT("Animation2D/"));
	AddPath(SCENE_PATH, TEXT("Scene/"));
	AddPath(MESH_PATH, TEXT("Mesh/"));

	return true;
}

bool CPathManager::AddPath(const std::string& Name,
	const TCHAR* Path, const std::string& BasePathName)
{
	if (FindPath(Name))
		return false;

	const PathInfo* BasePath = FindPath(BasePathName);

	PathInfo* Info = new PathInfo;

	if (BasePath)
		lstrcpy(Info->Path, BasePath->Path);

	lstrcat(Info->Path, Path);

#ifdef UNICODE

	// �����ڵ�� �Ǿ��ִ� ���ڿ��� ��Ƽ����Ʈ�� �ٲٱ� ���� ����
	// ���´�.
	int	PathLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		0, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
		Info->PathMultibyte, PathLength, 0, 0);

#else

	strcpy_s(Info->PathMultibyte, Info->Path);

#endif // UNICODE

	m_mapPath.insert(std::make_pair(Name, Info));

	return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
	auto	iter = m_mapPath.find(Name);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}
