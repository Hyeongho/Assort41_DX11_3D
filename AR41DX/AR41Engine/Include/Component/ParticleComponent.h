#pragma once

#include "PrimitiveComponent.h"

class CParticleComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CParticleComponent();
    CParticleComponent(const CParticleComponent& component);
    virtual ~CParticleComponent();

private:
    CSharedPtr<class CParticle>   m_Particle;
    std::vector<class CStructuredBuffer*>   m_vecStructuredBuffer;
    class CParticleConstantBuffer* m_CBuffer;
    float m_SpawnTime;
    float m_SpawnTimeMax;
    int m_SpawnCountMax;

public:
    //김범중 에디터 값 조정용
    CParticle* GetParticle()	const
    {
        return m_Particle;
    }
    float GetSpawnTime()	const
    {
        return m_SpawnTime;
    }
    float GetSpawnTimeMax()	const
    {
        return m_SpawnTimeMax;
    }
    int GetSpawnCountMax()	const
    {
        return m_SpawnCountMax;
    }
    void DeleteCurrentParticle();
    void ChangeParticle(const std::string& Name);
    //
    void SetSpawnTime(float Time);
    void SetSpawnCountMax(int Count);
    void SetParticle(const std::string& Name);
    void SetParticle(class CParticle* Particle);
    virtual void SetMaterial(int Slot, const std::string& Name);
    virtual void SetMaterial(int Slot, class CMaterial* Material);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CParticleComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

