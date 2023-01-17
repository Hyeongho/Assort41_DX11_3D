#pragma once
#include "Collider.h"
class CCollider2D :
    public CCollider
{
    friend class CGameObject;
    friend class CScene;

protected:
    CCollider2D();
    CCollider2D(const CCollider2D& component);
    virtual ~CCollider2D();

protected:
    ECollider2D_Type          m_Collider2DType;

public:
    ECollider2D_Type GetCollider2DType()    const
    {
        return m_Collider2DType;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CCollider2D* Clone()    const = 0;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest) = 0;
    virtual bool CollisionMouse(const Vector2& MouseWorldPos) = 0;
};

