#include "StaticMesh.h"
#include "../../PathManager.h"
#include "FBXLoader.h"

CStaticMesh::CStaticMesh()
{
    m_MeshType = MeshType::Static;

    SetTypeID<CStaticMesh>();
}

CStaticMesh::~CStaticMesh()
{
}

bool CStaticMesh::LoadMesh(const std::string& Name,
    const TCHAR* FileName, const std::string& PathName)
{
    TCHAR   FullPath[MAX_PATH] = {};

    const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

    if (Info)
        lstrcpy(FullPath, Info->Path);

    lstrcat(FullPath, FileName);

    return LoadMeshFullPath(Name, FullPath);
}

bool CStaticMesh::LoadMeshFullPath(const std::string& Name, 
    const TCHAR* FullPath)
{
    char   _FullPath[MAX_PATH] = {};

    int Length = WideCharToMultiByte(CP_ACP, 0, FullPath, -1,
        nullptr, 0, nullptr, nullptr);
    WideCharToMultiByte(CP_ACP, 0, FullPath, -1, _FullPath,
        Length, nullptr, nullptr);

    return LoadMeshMultibyteFullPath(Name, _FullPath);
}

bool CStaticMesh::LoadMeshMultibyte(const std::string& Name,
    const char* FileName, const std::string& PathName)
{
    char   FullPath[MAX_PATH] = {};

    const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

    if (Info)
        strcpy_s(FullPath, Info->PathMultibyte);

    strcat_s(FullPath, FileName);

    return LoadMeshMultibyteFullPath(Name, FullPath);
}

bool CStaticMesh::LoadMeshMultibyteFullPath(const std::string& Name,
    const char* FullPath)
{
    // FBX 파일인지를 판단한다.
    char    Ext[_MAX_EXT] = {};

    _splitpath_s(FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);
    _strupr_s(Ext);

    if (strcmp(Ext, ".FBX") == 0)
    {
        CFBXLoader  Loader;

        if (!Loader.LoadFBX(FullPath))
            return false;

        return ConvertFBX(&Loader, FullPath);
    }

    return LoadMeshFile(FullPath);
}
