#pragma once

#include "Collider3D.h"

class CColliderOBB3D :
    public CCollider3D
{
    friend class CGameObject;
    friend class CScene;

protected:
    CColliderOBB3D();
    CColliderOBB3D(const CColliderOBB3D& component);
    virtual ~CColliderOBB3D();

protected:
    Vector3 m_BoxHalfSize;
    OBB3DInfo m_Info;

public:
    const OBB3DInfo& GetInfo()  const
    {
        return m_Info;
    }

public:
    void SetBoxHalfSize(const Vector3& Size)
    {
        m_BoxHalfSize = Size;
    }

    void SetBoxHalfSize(float Width, float Height, float Depth)
    {
        m_BoxHalfSize.x = Width;
        m_BoxHalfSize.y = Height;
        m_BoxHalfSize.z = Depth;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual void RenderDebug();
    virtual CColliderOBB3D* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest);
};

