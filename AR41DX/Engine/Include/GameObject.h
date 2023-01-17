#pragma once

#include "Component/SceneComponent.h"
#include "Component/ObjectComponent.h"

class CGameObject :
	public CRef
{
	friend class CScene;

protected:
	CGameObject();
	CGameObject(const CGameObject& Obj);
	virtual ~CGameObject();

protected:
	class CScene* m_Scene;

public:
	class CScene* GetScene()    const
	{
		return m_Scene;
	}

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

public:
	// ���⿡ �������ϴ� ������ �� ������Ʈ�� ���ŵɶ� ������ �ִ� ��� ������Ʈ�鵵 ���Ÿ� ���ֱ�
	// ���ؼ��̴�.
	virtual void Destroy();

protected:
	CSharedPtr<CSceneComponent> m_RootComponent;
	std::list<CSceneComponent*> m_SceneComponentList;
	std::vector<CSharedPtr<CObjectComponent>>   m_vecObjectComponent;

	CGameObject* m_Parent;
	std::vector<CSharedPtr<CGameObject>>    m_vecChildObject;
	float       m_LifeTime;

public:
	void SetLifeTime(float LifeTime)
	{
		m_LifeTime = LifeTime;
	}

	void SetRootComponent(CSceneComponent* Component)
	{
		m_RootComponent = Component;
	}

	void AddSceneComponent(CSceneComponent* Component)
	{
		m_SceneComponentList.push_back(Component);
	}

	void DeleteSceneComponent(CSceneComponent* Component)
	{
		auto	iter = m_SceneComponentList.begin();
		auto	iterEnd = m_SceneComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			if (*iter == Component)
			{
				m_SceneComponentList.erase(iter);
				return;
			}
		}
	}

	CSceneComponent* GetRootComponent() const
	{
		return m_RootComponent;
	}

	const std::list<CSceneComponent*>& GetSceneComponents() const
	{
		return m_SceneComponentList;
	}

	CComponent* FindComponent(const std::string& Name);

	template <typename T>
	T* FindComponentFromType()  const
	{
		auto    iter = m_SceneComponentList.begin();
		auto    iterEnd = m_SceneComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->CheckTypeID<T>())
				return (T*)*iter;
		}

		auto    iter1 = m_vecObjectComponent.begin();
		auto    iter1End = m_vecObjectComponent.end();

		for (; iter1 != iter1End; ++iter1)
		{
			if ((*iter1)->CheckTypeID<T>())
				return (T*)(*iter1).Get();
		}

		return nullptr;
	}


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGameObject* Clone()    const;


public:
	template <typename T>
	T* CreateComponent(const std::string& Name)
	{
		T* Component = new T;

		Component->SetName(Name);
		Component->SetScene(m_Scene);
		Component->SetOwner(this);

		if(!Component->Init())
		{
			SAFE_RELEASE(Component);
			return nullptr;
		}

		if (Component->GetComponentType() == ComponentType::Object)
		{
			m_vecObjectComponent.push_back((CObjectComponent*)Component);
		}

		else
		{
			if (!m_RootComponent)
			{
				m_RootComponent = Component;
			}

			m_SceneComponentList.push_back((CSceneComponent*)Component);
		}

		return Component;
	}

public:
	void SetInheritScale(bool Inherit);
	void SetInheritRotX(bool Inherit);
	void SetInheritRotY(bool Inherit);
	void SetInheritRotZ(bool Inherit);
	void SetInheritParentRotationPosX(bool Inherit);
	void SetInheritParentRotationPosY(bool Inherit);
	void SetInheritParentRotationPosZ(bool Inherit);

	// �θ��� ������ �ڽ� Transform�� �����Ѵ�.
	void InheritScale();
	void InheritRotation(bool Current);
	void InheritParentRotationPos();

	void InheritWorldScale();
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
	const Vector3& GetWorldScale()	const;
	const Vector3& GetWorldRot()	const;
	const Vector3& GetWorldPos()	const;
	const Vector3& GetWorldAxis(AXIS Axis)	const;
	const Vector3& GetPivot()	const;
	const Vector3& GetMeshSize()	const;
	const Matrix& GetWorldMatrix()	const;

public:
	void SetPivot(const Vector3& Pivot);
	void SetPivot(const Vector2& Pivot);
	void SetPivot(float x, float y, float z);
	void SetPivot(float x, float y);
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
};

