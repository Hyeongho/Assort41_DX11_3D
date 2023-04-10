#pragma once

#include "Monster.h"

class CDupli_Can :
    public CMonster
{
    friend class CScene;

protected:
    CDupli_Can();
    CDupli_Can(const CDupli_Can& Obj);
    virtual ~CDupli_Can();

private:
    CSharedPtr<class CStaticMeshComponent>   m_Mesh;
    CSharedPtr<class CColliderOBB3D>   m_Cube;
    CSharedPtr<class CRigidBody>      m_Rigid;

private:
    float m_FallTime;
    int  m_CountHammer;
    bool m_DefyingGravity;
    bool m_SpawnOn;
    bool m_Land;

    CHammer* m_Hammers[3];

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CDupli_Can* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

private:
    void SpawnHammer();

public:
    void SetCountHammer(int CountHammer)
    {
        m_CountHammer = CountHammer;
    }

    int GetCountHammer()
    {
        return m_CountHammer;
    }

    void SetLand(int Land)
    {
        m_Land = Land;
    }

    int GetLand()
    {
        return m_Land;
    }

private: // Collider
    void Collision_Ground(const CollisionResult& result);
};
