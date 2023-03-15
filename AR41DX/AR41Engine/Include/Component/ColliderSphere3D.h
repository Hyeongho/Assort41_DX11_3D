#pragma once

#include "Collider3D.h"

class CColliderSphere3D :
    public CCollider3D
{
    friend class CGameObject;
    friend class CScene;

protected:
    CColliderSphere3D();
    CColliderSphere3D(const CColliderSphere3D& component);
    virtual ~CColliderSphere3D();

protected:
    float m_Radius;
    Sphere3DInfo m_Info;

public:
    const Sphere3DInfo& GetInfo() const
    {
        return m_Info;
    }

public:
    void SetRadius(float Radius)
    {
        m_Radius = Radius;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual void RenderDebug();
    virtual CColliderSphere3D* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest);
};

