#pragma once

#include "Collider3D.h"

class CColliderCube :
    public CCollider3D
{
    friend class CGameObject;
    friend class CScene;

protected:
    CColliderCube();
    CColliderCube(const CColliderCube& component);
    virtual ~CColliderCube();

protected:
    Vector3 m_CubeSize;
    CubeInfo m_Info;

public:
    Vector3 GetCubeSize() const
    {
        return m_CubeSize;
    }

    CubeInfo GetCubeInfo() const
    {
        return m_Info;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CColliderCube* Clone() const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest);
};

