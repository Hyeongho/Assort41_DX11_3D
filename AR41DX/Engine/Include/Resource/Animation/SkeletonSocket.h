#pragma once

#include "../../EngineInfo.h"

class CSkeletonSocket
{
	friend class CSkeleton;
	friend class CAnimation;

private:
	CSkeletonSocket();
	CSkeletonSocket(const CSkeletonSocket& Socket);
	~CSkeletonSocket();

private:
	std::string		m_Name;
	std::string		m_BoneName;
	int				m_BoneIndex;
	Vector3			m_Offset;
	Vector3			m_OffsetRotation;
	Matrix			m_matSocket;
	Vector3			m_BoneScale;
	Vector3			m_BonePos;
	Vector4			m_BoneRot;

	Vector3			m_WorldScale;
	Vector3			m_WorldPos;
	Vector3			m_WorldRot;

	Vector3			m_PrevWorldScale;
	Vector3			m_PrevWorldPos;
	Vector3			m_PrevWorldRot;

	Vector3			m_MoveScale;
	Vector3			m_MovePos;
	Vector3			m_MoveRot;

public:
	const Vector3& GetMoveScale()	const
	{
		return m_MoveScale;
	}

	const Vector3& GetMovePos()	const
	{
		return m_MovePos;
	}

	const Vector3& GetMoveRot()	const
	{
		return m_MoveRot;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	const std::string& GetBoneName()	const
	{
		return m_BoneName;
	}

	int GetBoneIndex()	const
	{
		return m_BoneIndex;
	}

	const Vector3& GetOffset()	const
	{
		return m_Offset;
	}

	const Vector3& GetOffsetRotation()	const
	{
		return m_OffsetRotation;
	}

	const Matrix& GetSocketMatrix()	const
	{
		return m_matSocket;
	}

	const Vector3& GetWorldScale()	const
	{
		return m_WorldScale;
	}

	const Vector3& GetWorldRot()	const
	{
		return m_WorldRot;
	}

	const Vector3& GetWorldPos()	const
	{
		return m_WorldPos;
	}

public:
	void SetOffset(const Vector3& Offset)
	{
		m_Offset = Offset;
	}

	void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}

	void SetOffsetRotation(const Vector3& OffsetRotation)
	{
		m_OffsetRotation = OffsetRotation;
	}

	void SetOffsetRotation(float x, float y, float z)
	{
		m_OffsetRotation = Vector3(x, y, z);
	}

	void SetBone(const std::string& BoneName)
	{
		m_BoneName = BoneName;
	}

	void SetBoneIndex(int Index)
	{
		m_BoneIndex = Index;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

public:
	void Update(const OutputBoneInfo& Info, class CAnimationMeshComponent* Owner);
	CSkeletonSocket* Clone();

private:
	void ComputeBone();
	void ComputeWorld(class CAnimationMeshComponent* Owner);
};

