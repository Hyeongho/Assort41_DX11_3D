#pragma once
#include "Collider.h"
class CCollider3D :
    public CCollider
{
    friend class CGameObject;
    friend class CScene;

protected:
    CCollider3D();
    CCollider3D(const CCollider3D& component);
    virtual ~CCollider3D();

protected:
    ECollider3D_Type m_Collider3DType;
    class CRenderState* m_WireFrame;


public:
    ECollider3D_Type GetCollider3DType() const
    {
        return m_Collider3DType;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CCollider3D* Clone()    const = 0;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest) = 0;
};

