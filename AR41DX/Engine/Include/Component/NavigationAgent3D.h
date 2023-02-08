#pragma once

#include "ObjectComponent.h"
#include "../Thread/ThreadQueue.h"

class CNavigationAgent3D :
    public CObjectComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CNavigationAgent3D();
    CNavigationAgent3D(const CNavigationAgent3D& Obj);
    virtual ~CNavigationAgent3D();

private:
    CSharedPtr<class CSceneComponent> m_UpdateComponent;
    CThreadQueue m_FindQueue;
    std::list<Vector3> m_PathList;
    float m_MoveSpeed;

public:
    class CSceneComponent* GetUpdateComponent() const
    {
        return m_UpdateComponent;
    }

public:
    void SetUpdateComponent(class CSceneComponent* Component);
    void SetMoveSpeed(float Speed);
    void AddPathList(const std::list<Vector3>& PathList);
    bool Move(const Vector3& TargetPos);

public:
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CNavigationAgent3D* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

