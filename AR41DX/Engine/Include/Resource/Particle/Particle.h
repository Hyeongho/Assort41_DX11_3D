#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"

class CParticle :
    public CRef
{
    friend class CParticleManager;
    friend class CResourceManager;
    friend class CSceneResource;

private:
    CParticle();
    ~CParticle();

private:
    class CScene* m_Scene;
    std::vector<class CStructuredBuffer*> m_vecStructuredBuffer;
    CSharedPtr<CMaterial> m_Material;

public:
    bool Init();
};

