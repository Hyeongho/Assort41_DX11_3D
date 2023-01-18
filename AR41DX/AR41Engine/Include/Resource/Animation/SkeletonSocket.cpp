#include "SkeletonSocket.h"
#include "../../Component/AnimationMeshComponent.h"

CSkeletonSocket::CSkeletonSocket()
{
}

CSkeletonSocket::CSkeletonSocket(const CSkeletonSocket& Socket)
{
	*this = Socket;
}

CSkeletonSocket::~CSkeletonSocket()
{
}

void CSkeletonSocket::Update(const OutputBoneInfo& Info, CAnimationMeshComponent* Owner)
{
	m_PrevWorldPos = m_WorldPos;
	m_PrevWorldScale = m_WorldScale;
	m_PrevWorldRot = m_WorldRot;

	// 소켓의 부모는 본이다.
	// 본의 부모는 오너의 월드이다.

	m_BoneScale = Info.Scale;
	m_BonePos = Info.Pos;
	m_BoneRot = Info.Rot;

	m_BoneRot.x = RadianToDegree(m_BoneRot.x);
	m_BoneRot.y = RadianToDegree(m_BoneRot.y);
	m_BoneRot.z = RadianToDegree(m_BoneRot.z);

	ComputeBone();

	ComputeWorld(Owner);

	Matrix	matScale, matRot, matPos;

	matScale.Scaling(m_WorldScale);
	matRot.Rotation(m_WorldRot);
	matPos.Translation(m_WorldPos);

	m_matSocket = matScale * matRot * matPos;

	m_MoveScale = m_WorldScale - m_PrevWorldScale;
	m_MovePos = m_WorldPos - m_PrevWorldPos;
	m_MoveRot = m_WorldRot - m_PrevWorldRot;

	if (m_MoveScale.x <= 0.00001f)
		m_MoveScale.x = 1.f;

	if (m_MoveScale.y <= 0.00001f)
		m_MoveScale.y = 1.f;

	if (m_MoveScale.z <= 0.00001f)
		m_MoveScale.z = 1.f;
}

CSkeletonSocket* CSkeletonSocket::Clone()
{
	return new CSkeletonSocket(*this);
}

void CSkeletonSocket::ComputeBone()
{
	// 본의 회전정보를 이용해서 Offset의 위치를 구해준다.
	// 만약 Offset이 0, 0, 0 이라면 연산은 필요없다.
	if (m_Offset.x != 0.f || m_Offset.y != 0.f || m_Offset.z != 0.f)
	{
		Matrix	matRot;
		/*Vector3	ParentRot = Vector3(m_BoneRot.x, m_BoneRot.y, m_BoneRot.z);

		DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ParentRot.x,
			ParentRot.y, ParentRot.z);*/

		// 행렬을 구한다.
		Vector4	BoneRot = m_BoneRot;
		BoneRot.x = DegreeToRadian(BoneRot.x);
		BoneRot.y = DegreeToRadian(BoneRot.y);
		BoneRot.z = DegreeToRadian(BoneRot.z);
		matRot.RotationQuaternion(BoneRot);

		memcpy(&matRot._41, &m_BonePos, sizeof(Vector3));

		m_WorldPos = m_Offset.TransformCoord(matRot);
	}

	else
	{
		m_WorldPos = m_BonePos;
	}

	m_WorldRot.x = m_OffsetRotation.x + m_BoneRot.x;
	m_WorldRot.y = m_OffsetRotation.y + m_BoneRot.y;
	m_WorldRot.z = m_OffsetRotation.z + m_BoneRot.z;

	m_WorldScale = m_BoneScale;
}

void CSkeletonSocket::ComputeWorld(CAnimationMeshComponent* Owner)
{
	Vector3	ParentRot = Owner->GetWorldRot();

	ParentRot = ParentRot.ConvertAngle();

	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ParentRot.x, ParentRot.y, ParentRot.z);

	// 행렬을 구한다.
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	Vector3	ParentPos = Owner->GetWorldPos();

	memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

	m_WorldPos = m_WorldPos.TransformCoord(matRot);

	m_WorldRot += Owner->GetWorldRot();

	m_WorldScale *= Owner->GetWorldScale();
}
