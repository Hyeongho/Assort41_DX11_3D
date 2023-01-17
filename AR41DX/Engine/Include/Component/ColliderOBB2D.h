#pragma once
#include "Collider2D.h"
class CColliderOBB2D :
    public CCollider2D
{
    friend class CGameObject;
    friend class CScene;

protected:
    CColliderOBB2D();
    CColliderOBB2D(const CColliderOBB2D& component);
    virtual ~CColliderOBB2D();

protected:
    Vector2     m_BoxHalfSize;
    OBB2DInfo   m_Info;

public:
    const OBB2DInfo& GetInfo()  const
    {
        return m_Info;
    }

public:
    void SetBoxHalfSize(const Vector2& Size)
    {
        m_BoxHalfSize = Size;
    }

    void SetBoxHalfSize(float Width, float Height)
    {
        m_BoxHalfSize.x = Width;
        m_BoxHalfSize.y = Height;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CColliderOBB2D* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest);
    virtual bool CollisionMouse(const Vector2& MouseWorldPos);
};

