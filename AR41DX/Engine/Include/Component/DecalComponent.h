#pragma once

#include "PrimitiveComponent.h"

enum class EDecalFadeState
{
    None,
    FadeIn,
    FadeOut,
    FadeInOut
};

enum class EFadeState
{
    FadeIn,
    Duration,
    FadeOut
};

class CDecalComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CDecalComponent();
    CDecalComponent(const CDecalComponent& component);
    virtual ~CDecalComponent();

private:
#ifdef _DEBUG
    CSharedPtr<class CStaticMesh> m_DebugMesh;
    CSharedPtr<class CMaterial> m_DebugMaterial;
#endif // _DEBUG
    EDecalFadeState m_FadeState;
    EFadeState m_Fade;
    float m_FadeTime;
    float m_FadeInTime;
    float m_FadeOutTime;
    float m_FadeOutDelayTime;
    bool m_FinishDelete;

public:
    void SetFadeState(EDecalFadeState State)
    {
        m_FadeState = State;
    }

    void SetFadeInTime(float Time)
    {
        m_FadeInTime = Time;
    }

    void SetFadeOutTime(float Time)
    {
        m_FadeOutTime = Time;
    }

    void SetFadeOutDelayTime(float Time)
    {
        m_FadeOutDelayTime = Time;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    void RenderDebug();
    virtual CDecalComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

