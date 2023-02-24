#pragma once

#include "Component.h"
#include "Transform.h"

class CSceneComponent :
    public CComponent
{
	friend class CGameObject;
	friend class CScene;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& component);
	virtual ~CSceneComponent();

protected:
	std::string m_SceneName;
	CTransform* m_Transform;
    CSceneComponent* m_Parent;
	class CSkeletonSocket* m_Socket;
	SceneComponentType	m_SceneComponentType;
    std::vector<CSharedPtr<CSceneComponent>> m_vecChild;
	std::string m_LayerName;
	std::string m_ParentName;
	std::vector<std::string> m_vecChildName;
	bool m_FrustumCull;

public:
	bool GetFrustumCull() const
	{
		return m_FrustumCull;
	}

	void SetFrustumCull(bool Cull)
	{
		m_FrustumCull = Cull;
	}

	const std::string& GetSceneName() const
	{
		return m_SceneName;
	}
	
	SceneComponentType GetSceneComponentType() const
	{
		return m_SceneComponentType;
	}

    CTransform* GetTransform() const
    {
        return m_Transform;
    }

	CSceneComponent* GetParent() const
	{
		return m_Parent;
	}

public:
	const std::string& GetRenderLayerName() const
	{
		return m_LayerName;
	}

	void SetRenderLayerName(const std::string& Name)
	{
		m_LayerName = Name;
	}

	void AddOwner();
	virtual void SetSocket(class CSkeletonSocket* Socket);

public:
    virtual void SetScene(class CScene* Scene);
    virtual void SetOwner(class CGameObject* Owner);

public:
	// SocketName은 나중에 3D에서 사용 예정.
	virtual void AddChild(CSceneComponent* Child, const std::string& SocketName = "");
	virtual void AddChild(class CGameObject* Child, const std::string& SocketName = "");
	bool DeleteChild(CSceneComponent* Child);
	bool DeleteChild(const std::string& Name);
	CSceneComponent* FindComponent(const std::string& Name);
	void GetAllComponentHierarchyName(std::vector<HierarchyName>& vecName);

public:
    virtual void Destroy();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CSceneComponent* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void SetInheritScale(bool Inherit);
	void SetInheritRotX(bool Inherit);
	void SetInheritRotY(bool Inherit);
	void SetInheritRotZ(bool Inherit);
	void SetInheritParentRotationPosX(bool Inherit);
	void SetInheritParentRotationPosY(bool Inherit);
	void SetInheritParentRotationPosZ(bool Inherit);

	// 부모의 정보를 자식 Transform에 적용한다.
	void InheritRotation(bool Current);
	void InheritParentRotationPos();

	void InheritWorldRotation(bool Current);
	void InheritWorldParentRotationPos();

public:
	Vector3 GetRelativeScale()	const;
	Vector3 GetRelativeRot()	const;
	Vector3 GetRelativePos()	const;
	Vector3 GetRelativeAxis(AXIS Axis)	const;

public:
	void SetRelativeScale(const Vector3& Scale);
	void SetRelativeScale(const Vector2& Scale);
	void SetRelativeScale(float x, float y);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeScaleX(float x);
	void SetRelativeScaleY(float y);
	void SetRelativeScaleZ(float z);
	void SetRelativeRotation(const Vector3& Rot);
	void SetRelativeRotation(const Vector2& Rot);
	void SetRelativeRotation(float x, float y);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativePosition(const Vector3& Pos);
	void SetRelativePosition(const Vector2& Pos);
	void SetRelativePosition(float x, float y);
	void SetRelativePosition(float x, float y, float z);
	void SetRelativePositionX(float x);
	void SetRelativePositionY(float y);
	void SetRelativePositionZ(float z);

	void AddRelativeScale(const Vector3& Scale);
	void AddRelativeScale(const Vector2& Scale);
	void AddRelativeScale(float x, float y);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeScaleX(float x);
	void AddRelativeScaleY(float y);
	void AddRelativeScaleZ(float z);
	void AddRelativeRotation(const Vector3& Rot);
	void AddRelativeRotation(const Vector2& Rot);
	void AddRelativeRotation(float x, float y);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);
	void AddRelativePosition(const Vector3& Pos);
	void AddRelativePosition(const Vector2& Pos);
	void AddRelativePosition(float x, float y);
	void AddRelativePosition(float x, float y, float z);
	void AddRelativePositionX(float x);
	void AddRelativePositionY(float y);
	void AddRelativePositionZ(float z);

public:
	const Vector3& GetCenter() const;
	const Vector3& GetMin()	const;
	const Vector3& GetMax()	const;
	float GetRadius() const;
	const Vector3& GetWorldScale() const;
	const Vector3& GetWorldRot() const;
	const Vector3& GetWorldPos() const;
	const Vector3& GetWorldAxis(AXIS Axis) const;
	const Vector3& GetPivot() const;
	const Vector3& GetMeshSize() const;
	const Vector3& GetOffset() const;
	const Matrix& GetWorldMatrix() const;

public:
	void SetPivot(const Vector3& Pivot);
	void SetPivot(const Vector2& Pivot);
	void SetPivot(float x, float y, float z);
	void SetPivot(float x, float y);
	void SetMin(const Vector3& Min);
	void SetMax(const Vector3& Max);
	void SetMeshSize(const Vector3& MeshSize);
	void SetMeshSize(const Vector2& MeshSize);
	void SetMeshSize(float x, float y, float z);
	void SetMeshSize(float x, float y);

public:
	void SetWorldScale(const Vector3& Scale);
	void SetWorldScale(const Vector2& Scale);
	void SetWorldScale(float x, float y);
	void SetWorldScale(float x, float y, float z);
	void SetWorldScaleX(float x);
	void SetWorldScaleY(float y);
	void SetWorldScaleZ(float z);
	void SetWorldRotation(const Vector3& Rot);
	void SetWorldRotation(const Vector2& Rot);
	void SetWorldRotation(float x, float y);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldPosition(const Vector3& Pos);
	void SetWorldPosition(const Vector2& Pos);
	void SetWorldPosition(float x, float y);
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPositionX(float x);
	void SetWorldPositionY(float y);
	void SetWorldPositionZ(float z);
	void SetOffset(const Vector3& Offset);
	void SetOffset(const Vector2& Offset);
	void SetOffsetX(float x);
	void SetOffsetY(float y);
	void SetOffsetZ(float z);

	void AddWorldScale(const Vector3& Scale);
	void AddWorldScale(const Vector2& Scale);
	void AddWorldScale(float x, float y);
	void AddWorldScale(float x, float y, float z);
	void AddWorldScaleX(float x);
	void AddWorldScaleY(float y);
	void AddWorldScaleZ(float z);
	void AddWorldRotation(const Vector3& Rot);
	void AddWorldRotation(const Vector2& Rot);
	void AddWorldRotation(float x, float y);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);
	void AddWorldPosition(const Vector3& Pos);
	void AddWorldPosition(const Vector2& Pos);
	void AddWorldPosition(float x, float y);
	void AddWorldPosition(float x, float y, float z);
	void AddWorldPositionX(float x);
	void AddWorldPositionY(float y);
	void AddWorldPositionZ(float z);
	void AddOffset(const Vector3& Offset);
	void AddOffset(const Vector2& Offset);
	void AddOffsetX(float x);
	void AddOffsetY(float y);
	void AddOffsetZ(float z);
};

