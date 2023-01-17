#pragma once

#include "../Ref.h"

class CComponent :
    public CRef
{
    friend class CGameObject;
    friend class CSceneManager;
    friend class CScene;

protected:
    CComponent();
    CComponent(const CComponent& Obj);
    virtual ~CComponent() = 0;

private:
    static std::unordered_map<std::string, CComponent*> m_mapComponent;

public:
    static void AddComponentCDO(const std::string& Name, CComponent* CDO)
    {
        m_mapComponent.insert(std::make_pair(Name, CDO));
    }

    static CComponent* FindCDO(const std::string& Name)
    {
        auto	iter = m_mapComponent.find(Name);

        if (iter == m_mapComponent.end())
            return nullptr;

        return iter->second;
    }

protected:
    ComponentType   m_ComponentType;
    class CScene* m_Scene;
    class CGameObject* m_Owner;
    std::string     m_ComponentTypeName;
    int             m_SerialNumber; // 게임오브젝트에 포함될때 사용할 식별번호.

public:
    void SetSerialNumber(int Number)
    {
        m_SerialNumber = Number;
    }

    int GetSerialNumber()   const
    {
        return m_SerialNumber;
    }

    const std::string& GetComponentTypeName()   const
    {
        return m_ComponentTypeName;
    }

    class CScene* GetScene()    const
    {
        return m_Scene;
    }

    class CGameObject* GetOwner()   const
    {
        return m_Owner;
    }

    virtual void SetScene(class CScene* Scene)
    {
        m_Scene = Scene;
    }

    virtual void SetOwner(class CGameObject* Owner)
    {
        m_Owner = Owner;
    }

    ComponentType GetComponentType()    const
    {
        return m_ComponentType;
    }

public:
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

