#pragma once

#include "PrimitiveComponent.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Animation/Animation.h"
#include "../Resource/Mesh/AnimationMesh.h"

class CAnimationMeshComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CAnimationMeshComponent();
    CAnimationMeshComponent(const CAnimationMeshComponent& component);
    virtual ~CAnimationMeshComponent();

protected:
    CSharedPtr<CSkeleton>   m_Skeleton;
    CSharedPtr<CAnimation>  m_Animation;

public:
    std::string GetAnimationName()  const
    {
        if (!m_Animation)
            return "None";

        return m_Animation->GetAnimationClassName();
    }

    CAnimation* GetAnimation()    const
    {
        return m_Animation;
    }

    CSkeleton* GetSkeleton()    const
    {
        return m_Skeleton;
    }

    void GetAnimationNames(std::vector<std::string>& vecNames);

public:
    //김범중 애니메이션 교체용 함수
    void SetAnimation(class CAnimation* anim);

    virtual bool SetMesh(const std::string& Name);
    virtual bool SetMesh(class CMesh* Mesh);
    virtual bool SetMesh(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& Name, const TCHAR* FullPath);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CAnimationMeshComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    template <typename T>
    T* SetAnimation(const std::string& Name)
    {
        T* Anim = new T;

        Anim->SetName(Name);
        Anim->m_Owner = this;

        if (!Anim->Init())
        {
            SAFE_DELETE(Anim);
            return nullptr;
        }

        m_Animation = Anim;

        if (m_Skeleton)
            m_Animation->SetSkeleton(m_Skeleton);

        if (m_Mesh)
            m_Animation->SetInstancingBoneBuffer(((CAnimationMesh*)m_Mesh.Get())->GetBoneBuffer());

        return Anim;
    }
};

