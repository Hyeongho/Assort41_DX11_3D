#pragma once

#include "PrimitiveComponent.h"

class CBillboardComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CBillboardComponent();
    CBillboardComponent(const CBillboardComponent& component);
    virtual ~CBillboardComponent();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CBillboardComponent* Clone() const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

