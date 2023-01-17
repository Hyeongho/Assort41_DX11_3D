#pragma once
#include "Collider2D.h"
class CColliderSphere2D :
    public CCollider2D
{
    friend class CGameObject;
    friend class CScene;

protected:
    CColliderSphere2D();
    CColliderSphere2D(const CColliderSphere2D& component);
    virtual ~CColliderSphere2D();

protected:
    float           m_Radius;
    Sphere2DInfo    m_Info;

public:
    const Sphere2DInfo& GetInfo()  const
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
    virtual CColliderSphere2D* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionMouse(const Vector2& MouseWorldPos);
};

