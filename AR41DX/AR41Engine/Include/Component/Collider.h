#pragma once
#include "SceneComponent.h"
class CCollider :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CScene;
    friend class CCollisionManager;

protected:
    CCollider();
    CCollider(const CCollider& component);
    virtual ~CCollider();

protected:
    CSharedPtr<class CMesh> m_Mesh;
    CSharedPtr<class CShader> m_Shader;
    Vector4                 m_Color;
    ECollider_Type          m_ColliderType;
    Vector3                 m_Min;
    Vector3                 m_Max;
    CollisionResult         m_Result;
    CollisionResult         m_MouseResult;
    std::list<CCollider*>   m_PrevCollisionList;    // ���� �����ӿ� �浹�ϰ� �ִ� ��ü��
    std::list<int>          m_CurrentSectionList;   // ���� �浹ü�� ��� �浹������ �����ִ��� �Ǵ��ϱ� ���� ����.
    CollisionProfile* m_Profile;
    Vector3                 m_HitPoint;
    bool                    m_MouseCollision;


    std::function<void(const CollisionResult&)> m_CollisionCallback[(int)ECollision_Result::Max];
    std::function<void(const CollisionResult&)> m_CollisionMouseCallback[(int)ECollision_Result::Max];

public:
    const Vector3& GetMin() const
    {
        return m_Min;
    }

    const Vector3& GetMax() const
    {
        return m_Max;
    }

    ECollider_Type GetColliderType()    const
    {
        return m_ColliderType;
    }

    CollisionProfile* GetCollisionProfile()  const
    {
        return m_Profile;
    }

public:
    void AddSectionIndex(int Index)
    {
        m_CurrentSectionList.push_back(Index);
    }

    void AddPrevCollisionCollider(CCollider* Collider)
    {
        m_PrevCollisionList.push_back(Collider);
    }

    void DeletePrevCollisionCollider(CCollider* Collider);

    bool CheckPrevCollision(CCollider* Collider);

    // ���� �����ӿ� �浹�ߴ� �浹ü���� �ݺ��ϸ� ���� ���� ������ ��ġ���� �Ǵ��Ѵ�.
    void CheckPrevCollisionColliderSection();

    // ���� �����ӿ� �浹�Ǵ� ��ü�鿡�� �浹������ �뺸�Ѵ�
    void SendPrevCollisionEnd();

    void CallCollisionCallback(ECollision_Result Type);
    void CallCollisionMouseCallback(ECollision_Result Type);
    void ClearFrame();
    // �浹 ������ ��ġ�°� 2�� �̻��� ��� �̹� �տ��� �浹ó���� �������̱� ������ ó���� ���ϵ��� �Ѵ�.
    // ��ȯ�� ��ġ�� �浹������ 2�� �̸��̸� -1, 2�� �̻��̸� ��ġ�� ������ ���� ���� �ε����� ��ȯ�Ѵ�.
    int CheckOverlapSection(CCollider* Dest);
    void SetCollisionProfile(const std::string& Name);

    void SetCollisionResultDest(CCollider* Dest)
    {
        m_Result.Dest = Dest;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CCollider* Clone()  const = 0;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest) = 0;

public:
    template <typename T>
    void SetCollisionCallback(ECollision_Result Type, T* Obj, void(T::* Func)(const CollisionResult&))
    {
        m_CollisionCallback[(int)Type] = std::bind(Func, Obj, std::placeholders::_1);
    }

    template <typename T>
    void SetCollisionMouseCallback(ECollision_Result Type, T* Obj, void(T::* Func)(const CollisionResult&))
    {
        m_CollisionMouseCallback[(int)Type] = std::bind(Func, Obj, std::placeholders::_1);
    }
};

