#pragma once
#include "Component.h"
class CObjectComponent :
    public CComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CObjectComponent();
    CObjectComponent(const CObjectComponent& Obj);
    virtual ~CObjectComponent() = 0;

public:
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CObjectComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

