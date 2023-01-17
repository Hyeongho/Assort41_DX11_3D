#pragma once
#include "Collider2D.h"
class CColliderBox2D :
    public CCollider2D
{
    friend class CGameObject;
    friend class CScene;

protected:
    CColliderBox2D();
    CColliderBox2D(const CColliderBox2D& component);
    virtual ~CColliderBox2D();

protected:
    Vector2     m_BoxSize;
    Box2DInfo   m_Info;

public:
    const Box2DInfo& GetInfo()  const
    {
        return m_Info;
    }

public:
    void SetBoxSize(const Vector2& Size)
    {
        m_BoxSize = Size;
    }

    void SetBoxSize(float Width, float Height)
    {
        m_BoxSize.x = Width;
        m_BoxSize.y = Height;
    }

public:
    Vector2 GetBoxSize()    const
    {
        return m_BoxSize;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CColliderBox2D* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionMouse(const Vector2& MouseWorldPos);
};

