#pragma once

#include "PrimitiveComponent.h"

class CTerrainComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CTerrainComponent();
    CTerrainComponent(const CTerrainComponent& component);
    virtual ~CTerrainComponent();

private:
    int m_CountX;
    int m_CountY;
    Vector2 m_CellSize;
    Vector2 m_Size;

public:
    virtual bool SetMesh(const std::string& Name);
    virtual bool SetMesh(class CMesh* Mesh);
    virtual bool SetMesh(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& Name, const TCHAR* FullPath);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CTerrainComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


public:
    void CreateTerrain(int CountX, int CountY, float SizeX, float SizeY, const char* HeightMapName = nullptr, 
        const std::string& HeightMapPath = TEXTURE_PATH);
};

