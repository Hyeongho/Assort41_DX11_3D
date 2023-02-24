#pragma once

#include "PrimitiveComponent.h"

class CStaticMeshComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CStaticMeshComponent();
    CStaticMeshComponent(const CStaticMeshComponent& component);
    virtual ~CStaticMeshComponent();

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
    virtual void RenderShadowMap();
    virtual CStaticMeshComponent* Clone() const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

