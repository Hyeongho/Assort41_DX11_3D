#pragma once
#include "SceneComponent.h"
class CPrimitiveComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CPrimitiveComponent();
    CPrimitiveComponent(const CPrimitiveComponent& component);
    virtual ~CPrimitiveComponent();

protected:
    CSharedPtr<class CMesh> m_Mesh;
    std::vector<CSharedPtr<class CMaterial>> m_vecMaterial;
    int m_InstanceID;
    bool m_Render;
    bool m_ReceiveDecal;

public:
    bool GetReceiveDecal()	const
    {
        return m_ReceiveDecal;
    }

    class CMesh* GetMesh()  const
    {
        return m_Mesh;
    }

    bool IsRender() const
    {
        return m_Render;
    }

    int GetInstanceID() const
    {
        return m_InstanceID;
    }

    void SetInstanceID(int ID)
    {
        m_InstanceID = ID;
    }

    class CMaterial* GetMaterial(int Slot)
    {
        return m_vecMaterial[Slot];
    }

    const std::vector<CSharedPtr<class CMaterial>>* GetMaterials()  const
    {
        return &m_vecMaterial;
    }

    int GetSlotCount()  const
    {
        return (int)m_vecMaterial.size();
    }

public:
    void SetReceiveDecal(bool Decal);

public:
    virtual bool SetMesh(const std::string& Name);
    virtual bool SetMesh(class CMesh* Mesh);
    virtual bool SetMesh(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& Name, const TCHAR* FullPath);
    virtual void SetMaterial(int Slot, const std::string& Name);
    virtual void SetMaterial(int Slot, class CMaterial* Material);
    virtual void AddMaterial(const std::string& Name);
    virtual void AddMaterial(class CMaterial* Material);
    virtual void ClearMaterial();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CPrimitiveComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

