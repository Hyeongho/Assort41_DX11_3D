#pragma once

#include "../EngineInfo.h"

class CTransform
{
	friend class CSceneComponent;

private:
	CTransform();
	CTransform(const CTransform& transform);
	~CTransform();

private:
	class CScene* m_Scene;
	class CGameObject* m_Object;
	class CSceneComponent* m_Owner;

private:
	bool m_Is2D;
	CTransform* m_Parent;
	std::vector<CTransform*> m_vecChild;
	class CTransformConstantBuffer* m_CBuffer;
	static float m_MinY;
	static float m_MaxY;
	float m_2DZ;
	class CSkeletonSocket* m_Socket;

private:
	bool m_InheritScale;
	bool m_InheritRotX;
	bool m_InheritRotY;
	bool m_InheritRotZ;
	bool m_InheritParentRotationPosX;
	bool m_InheritParentRotationPosY;
	bool m_InheritParentRotationPosZ;
	bool m_UpdateScale;
	bool m_UpdateRot;
	bool m_UpdateRotAxis;
	bool m_UpdatePos;

public:
	void SetSocket(class CSkeletonSocket* Socket)
	{
		m_Socket = Socket;
	}

	void Set2D(bool Is2D)
	{
		m_Is2D = Is2D;
	}

	void Clear();

	void SetInheritScale(bool Inherit)
	{
		m_InheritScale = Inherit;
	}

	void SetInheritRotX(bool Inherit)
	{
		m_InheritRotX = Inherit;
	}

	void SetInheritRotY(bool Inherit)
	{
		m_InheritRotY = Inherit;
	}

	void SetInheritRotZ(bool Inherit)
	{
		m_InheritRotZ = Inherit;
	}

	void SetInheritParentRotationPosX(bool Inherit)
	{
		m_InheritParentRotationPosX = Inherit;
	}

	void SetInheritParentRotationPosY(bool Inherit)
	{
		m_InheritParentRotationPosY = Inherit;
	}

	void SetInheritParentRotationPosZ(bool Inherit)
	{
		m_InheritParentRotationPosZ = Inherit;
	}

	// 부모의 정보를 자식 Transform에 적용한다.
	void InheritRotation(bool Current);
	void InheritParentRotationPos();

	void InheritWorldRotation(bool Current);
	void InheritWorldParentRotationPos();

private:	// Relative
	Vector3 m_RelativeScale;
	Vector3 m_RelativeRot;
	Vector3 m_RelativePos;
	Vector3 m_RelativeAxis[AXIS_MAX];

public:
	Vector3 GetRelativeScale()	const
	{
		return m_RelativeScale;
	}

	Vector3 GetRelativeRot()	const
	{
		return m_RelativeRot;
	}

	Vector3 GetRelativePos()	const
	{
		return m_RelativePos;
	}

	Vector3 GetRelativeAxis(AXIS Axis)	const
	{
		return m_RelativeAxis[Axis];
	}

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

private:
	void SetChildRelativeScale(const Vector3& Scale);
	void SetChildRelativeRotation(const Vector3& Rot);
	void SetChildRelativePosition(const Vector3& Pos);

	void AddChildRelativeScale(const Vector3& Scale);
	void AddChildRelativeRotation(const Vector3& Rot);
	void AddChildRelativePosition(const Vector3& Pos);


private:	// World
	Vector3 m_WorldScale;
	Vector3 m_WorldRot;
	Vector3 m_WorldPos;
	Vector3 m_WorldAxis[AXIS_MAX];
	Vector3 m_OriginWorldScale;
	Vector3 m_OriginWorldRot;
	Vector3 m_OriginWorldPos;
	Vector3 m_Pivot;
	Vector3 m_MeshSize;
	Vector3 m_Offset;

	Matrix m_matScale;
	Matrix m_matRot;
	Matrix m_matPos;
	Matrix m_matWorld;

public:
	const Vector3& GetWorldScale() const
	{
		return m_WorldScale;
	}

	const Vector3& GetWorldRot() const
	{
		return m_WorldRot;
	}

	const Vector3& GetWorldPos() const
	{
		return m_WorldPos;
	}

	const Vector3& GetWorldAxis(AXIS Axis) const
	{
		return m_WorldAxis[Axis];
	}

	const Vector3& GetPivot() const
	{
		return m_Pivot;
	}

	const Vector3& GetMeshSize() const
	{
		return m_MeshSize;
	}

	const Vector3& GetOffset() const
	{
		return m_Offset;
	}

	const Matrix& GetWorldMatrix() const
	{
		return m_matWorld;
	}

public:
	void SetPivot(const Vector3& Pivot)
	{
		m_Pivot = Pivot;
	}

	void SetPivot(const Vector2& Pivot)
	{
		m_Pivot.x = Pivot.x;
		m_Pivot.y = Pivot.y;
	}

	void SetPivot(float x, float y, float z)
	{
		m_Pivot = Vector3(x, y, z);
	}

	void SetPivot(float x, float y)
	{
		m_Pivot.x = x;
		m_Pivot.y = y;
	}

	void SetMeshSize(const Vector3& MeshSize)
	{
		m_MeshSize = MeshSize;
	}

	void SetMeshSize(const Vector2& MeshSize)
	{
		m_MeshSize.x = MeshSize.x;
		m_MeshSize.y = MeshSize.y;
	}

	void SetMeshSize(float x, float y, float z)
	{
		m_MeshSize = Vector3(x, y, z);
	}

	void SetMeshSize(float x, float y)
	{
		m_MeshSize.x = x;
		m_MeshSize.y = y;
	}


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
	void SetWorldRotationAxis(const Vector3& OriginDir, const Vector3& View);
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

private:
	void SetChildWorldScale(const Vector3& Scale);
	void SetChildWorldRotation(const Vector3& Rot);
	void SetChildWorldPosition(const Vector3& Pos);

	void AddChildWorldScale(const Vector3& Scale);
	void AddChildWorldRotation(const Vector3& Rot);
	void AddChildWorldPosition(const Vector3& Pos);

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	CTransform* Clone()    const;
	void SetTransform();	// 트랜스폼 정보를 Shader로 보내준다.
	void ComputeWorld();	// World정보를 계산한다.
	void Save(FILE* File);
	void Load(FILE* File);
};

