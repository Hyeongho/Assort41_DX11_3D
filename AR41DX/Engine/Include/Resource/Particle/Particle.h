#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"

class CParticle :
    public CRef
{
    friend class CParticleManager;
    friend class CResourceManager;
    friend class CSceneResource;
    friend class CParticleComponent;

private:
    CParticle();
    ~CParticle();

private:
    class CScene* m_Scene;
    std::vector<class CStructuredBuffer*> m_vecStructuredBuffer;
    CSharedPtr<CMaterial> m_Material;
    class CParticleConstantBuffer* m_CBuffer;
    CSharedPtr<class CParticleUpdateShader> m_UpdateShader;
    CSharedPtr<class CParticleRenderShader> m_RenderShader;
    float m_SpawnTime;

public:
    void SetUpdateShader(const std::string& Name);
    void SetRenderShader(const std::string& Name);
    void AddStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count, int Register, bool Dynamic = false, 
        int StructuredBufferShaderType = (int)EShaderBufferType::Compute);
    void ResizeStructuredBuffer(const std::string& Name, unsigned int Size, unsigned int Count, int Register, bool Dynamic = false,
        int StructuredBufferShaderType = (int)EShaderBufferType::Compute);
    void CloneStructuredBuffer(std::vector<class CStructuredBuffer*>& vecBuffer);
    class CParticleConstantBuffer* CloneCBuffer()   const;
    void SetParticleSpawnCountMax(int Count);
    void SetParticleSpawnTime(float Time);
    void SetMaterial(const std::string& Name);
    void SetMaterial(class CMaterial* Material);
    void SetParticleEnable(bool Enable);
    void SetParticleStartMin(const Vector3& Min);
    void SetParticleStartMax(const Vector3& Max);
    void SetParticleScaleMin(const Vector3& Min);
    void SetParticleScaleMax(const Vector3& Max);
    void SetParticleLifeTimeMin(float Min);
    void SetParticleLifeTimeMax(float Max);
    void SetParticleColorMin(const Vector4& Min);
    void SetParticleColorMax(const Vector4& Max);
    void SetParticleSpeedMin(float Min);
    void SetParticleSpeedMax(float Max);
    void SetParticleMoveEnable(bool Enable);
    void SetParticleGravityEnable(bool Enable);
    void SetParticleMoveDir(const Vector3& MoveDir);
    void SetParticleMoveDirEnable(bool Enable);
    void SetParticleMoveAngle(const Vector3& MoveAngle);

public:
    bool Init();
};

