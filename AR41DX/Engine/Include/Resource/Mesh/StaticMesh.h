#pragma once
#include "Mesh.h"
class CStaticMesh :
    public CMesh
{
    friend class CMeshManager;

protected:
    CStaticMesh();
    virtual ~CStaticMesh();

public:
	virtual bool LoadMesh(const std::string& Name,
		const TCHAR* FileName, const std::string& PathName = MESH_PATH);
	virtual bool LoadMeshFullPath(const std::string& Name,
		const TCHAR* FullPath);
	virtual bool LoadMeshMultibyte(const std::string& Name,
		const char* FileName, const std::string& PathName = MESH_PATH);
	virtual bool LoadMeshMultibyteFullPath(const std::string& Name,
		const char* FullPath);
};

