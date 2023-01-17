#pragma once

#include "ObjectComponent.h"
#include "../Thread/ThreadQueue.h"

class CNavigationAgent :
    public CObjectComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CNavigationAgent();
    CNavigationAgent(const CNavigationAgent& Obj);
    virtual ~CNavigationAgent();

private:
    CSharedPtr<class CSceneComponent> m_UpdateComponent;
    CThreadQueue	m_FindQueue;
    std::list<Vector2>  m_PathList;
    float           m_MoveSpeed;

public:
    class CSceneComponent* GetUpdateComponent() const
    {
        return m_UpdateComponent;
    }

public:
    void SetUpdateComponent(class CSceneComponent* Component);
    void SetMoveSpeed(float Speed);
    void AddPathList(const std::list<Vector2>& PathList);
    bool Move(const Vector2& TargetPos);

public:
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CNavigationAgent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

