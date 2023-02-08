#include "Transform.h"
#include "../Resource/Shader/TransformConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"
#include "../Resource/Animation/SkeletonSocket.h"

float CTransform::m_MinY = FLT_MAX;
float CTransform::m_MaxY = -FLT_MAX;

CTransform::CTransform()	:
	m_Is2D(false),
	m_InheritScale(false),
	m_InheritRotX(false),
	m_InheritRotY(false),
	m_InheritRotZ(false),
	m_InheritParentRotationPosX(true),
	m_InheritParentRotationPosY(true),
	m_InheritParentRotationPosZ(true),
	m_UpdateScale(true),
	m_UpdateRot(true),
	m_UpdateRotAxis(false),
	m_UpdatePos(true),
	m_RelativeScale(1.f, 1.f, 1.f),
	m_WorldScale(1.f, 1.f, 1.f),
	m_Parent(nullptr),
	m_Scene(nullptr),
	m_Object(nullptr),
	m_Owner(nullptr),
	m_Socket(nullptr),
	m_MeshSize(1.f, 1.f, 1.f),
	m_2DZ(0.f)
{
	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_RelativeAxis[i] = Vector3::Axis[i];
		m_WorldAxis[i] = Vector3::Axis[i];
	}

	m_CBuffer = new CTransformConstantBuffer;

	m_CBuffer->Init();
}

CTransform::CTransform(const CTransform& transform)
{
	*this = transform;

	m_CBuffer = transform.m_CBuffer->Clone();

	m_Socket = nullptr;
}

CTransform::~CTransform()
{
	SAFE_DELETE(m_CBuffer);
}

void CTransform::Clear()
{
	if (m_Is2D)
	{
		m_MinY = FLT_MAX;
		m_MaxY = -FLT_MAX;
	}
}

void CTransform::InheritRotation(bool Current)
{
	// 부모가 있을 경우 부모의 회전에 영향을 받는다.
	// 단, x, y, z 축 회전에 모두 따로 영향을 받을지 말지를 정할 수 있게 해두었으므로
	// 각 축마다 계산한다.
	if (m_Parent)
	{
		// x, y, z 축 회전중 하나라도 부모에 영향을 받고 현재 트랜스폼이 아닌 회전이 일어난
		// 트랜스폼의 자식 트랜스폼일 경우 위치가 변경이 되어야 한다.
		if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !Current)
		{
			InheritParentRotationPos();
		}
	}

	// x, y, z 축 회전 각도를 이용하여 실제 회전행렬을 구하기 위한 사원수를 만들어낸다.
	// Degree를 Radian으로 변환한다.
	Vector3	ConvertRot = m_RelativeRot.ConvertAngle();

	// 사원수를 구한다.
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	// 행렬을 구한다.
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	// 만들어준 상대적인 회전행렬을 이용해서 상대적인 축을 회전시킨다.
	// 월드행렬을 구성할때 41, 42, 43 부분에 위치가 들어간다.
	// 축은 위치가 필요 없는 방향만 존재하므로 위치에 영향을 안받고 회전에만 영향을 받게
	// 곱해주면 된다. 즉, x, y, z 가 있을 경우 4x4 행렬과 곱하기 위해 w를 0으로 만든다면
	// 행렬의 41, 42, 43에 영향을 안받고 곱해지게 되므로 위치에 영향을 안받을때는 w를 0으로,
	// 영향을 받을때는 w를 1로 해서 곱하면 된다.
	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertRot = m_WorldRot.ConvertAngle();

	// 사원수를 구한다.
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	// 행렬을 구한다.
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// 회전에 따라 축까지 모두 완료되었으면 자식노드를 반복하며 연산한다.
	m_UpdateRot = true;

	// 모든 자식노드를 갱신한다.
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritRotation(false);
	}
}

void CTransform::InheritParentRotationPos()
{
	// 부모가 있을 경우 부모의 위치에 영향을 받고 회전에도 영향을 받아 계산한다.
	if (m_Parent)
	{
		Matrix	matRot;
		Vector3	ParentRot;

		if (m_InheritRotX)
		{
			ParentRot.x = m_Parent->GetWorldRot().x;
		}

		if (m_InheritRotY)
		{
			ParentRot.y = m_Parent->GetWorldRot().y;
		}

		if (m_InheritRotZ)
		{
			ParentRot.z = m_Parent->GetWorldRot().z;
		}

		if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
		{
			Vector3	ConvertRot = ParentRot.ConvertAngle();

			DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

			// 행렬을 구한다.
			Matrix	matRot;
			matRot.RotationQuaternion(Qut);

			// 부모의 위치를 행렬의 41, 42, 43 에 지정한다.
			Vector3	ParentPos = m_Parent->GetWorldPos();

			memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

			Vector3	RelativePos = m_RelativePos + m_Offset;

			m_WorldPos = RelativePos.TransformCoord(matRot);
		}

		else
		{
			m_WorldPos = m_RelativePos + m_Parent->GetWorldPos() + m_Offset;
		}
	}

	m_UpdatePos = true;

	// 모든 자식노드를 갱신한다.
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritParentRotationPos();
	}
}

void CTransform::InheritWorldRotation(bool Current)
{
	// 부모가 있을 경우 부모의 회전에 영향을 받는다.
	// 단, x, y, z 축 회전에 모두 따로 영향을 받을지 말지를 정할 수 있게 해두었으므로
	// 각 축마다 계산한다.
	if (m_Parent)
	{
		// x, y, z 축 회전중 하나라도 부모에 영향을 받고 현재 트랜스폼이 아닌 회전이 일어난
		// 트랜스폼의 자식 트랜스폼일 경우 위치가 변경이 되어야 한다.
		if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !Current)
		{
			InheritWorldParentRotationPos();
		}
	}

	else
	{
		m_WorldRot = m_RelativeRot;
	}

	// x, y, z 축 회전 각도를 이용하여 실제 회전행렬을 구하기 위한 사원수를 만들어낸다.
	// Degree를 Radian으로 변환한다.
	Vector3	ConvertRot = m_RelativeRot.ConvertAngle();

	// 사원수를 구한다.
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	// 행렬을 구한다.
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	// 만들어준 상대적인 회전행렬을 이용해서 상대적인 축을 회전시킨다.
	// 월드행렬을 구성할때 41, 42, 43 부분에 위치가 들어간다.
	// 축은 위치가 필요 없는 방향만 존재하므로 위치에 영향을 안받고 회전에만 영향을 받게
	// 곱해주면 된다. 즉, x, y, z 가 있을 경우 4x4 행렬과 곱하기 위해 w를 0으로 만든다면
	// 행렬의 41, 42, 43에 영향을 안받고 곱해지게 되므로 위치에 영향을 안받을때는 w를 0으로,
	// 영향을 받을때는 w를 1로 해서 곱하면 된다.
	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertRot = m_WorldRot.ConvertAngle();

	// 사원수를 구한다.
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	// 행렬을 구한다.
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// 회전에 따라 축까지 모두 완료되었으면 자식노드를 반복하며 연산한다.
	m_UpdateRot = true;

	// 모든 자식노드를 갱신한다.
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritWorldRotation(false);
	}
}

void CTransform::InheritWorldParentRotationPos()
{
	// 부모가 있을 경우 부모의 위치에 영향을 받고 회전에도 영향을 받아 계산한다.
	if (m_Parent)
	{
		Matrix	matRot;
		Vector3	ParentRot;

		if (m_Owner->GetName() == "RightChild")
		{
			int a = 0;
		}

		if (m_InheritRotX)
		{
			ParentRot.x = m_Parent->GetWorldRot().x;
		}

		if (m_InheritRotY)
		{
			ParentRot.y = m_Parent->GetWorldRot().y;
		}

		if (m_InheritRotZ)
		{
			ParentRot.z = m_Parent->GetWorldRot().z;
		}

		if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
		{
			// 부모로부터 회전한 위치로 적용이 되기 때문에 월드위치를 강제할 경우 해당 위치로부터 
			// 현재의 상대적인 위치를 구한 후에 부모 회전의 역회전을 이용하여 최종 상대적인
			// 위치를 구해주어야 한다.
			//Vector3	RelativePos = m_WorldPos - m_Parent->GetWorldPos();

			// 역회전.

			Vector3	ConvertRot = ParentRot.ConvertAngle();

			DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
				ConvertRot.y, ConvertRot.z);

			// 행렬을 구한다.
			Matrix	matRot;
			matRot.RotationQuaternion(Qut);

			// 부모의 위치를 행렬의 41, 42, 43 에 지정한다.
			Vector3	ParentPos = m_Parent->GetWorldPos();

			memcpy(&matRot._41, &ParentPos, sizeof(Vector3));

			Vector3	RelativePos = m_RelativePos + m_Offset;

			m_WorldPos = RelativePos.TransformCoord(matRot);
		}

		else
		{
			m_RelativePos = m_WorldPos - m_Parent->GetWorldPos() - m_Offset;
		}
	}

	m_UpdatePos = true;

	// 모든 자식노드를 갱신한다.
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->InheritParentRotationPos();
	}
}

void CTransform::SetRelativeScale(const Vector3& Scale)
{
	m_RelativeScale = Scale;

	// 부모가 없을 경우라면 월드공간에서의 크기는 상대적인 크기와 동일한 크기로 적용을 한다.
	if (!m_Parent)
	{
		m_WorldScale = Scale;
	}

	else
	{
		if (m_InheritScale)
		{
			m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
		}

		else
		{
			m_WorldScale = m_RelativeScale;
		}
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildRelativeScale(Scale);
	}

	m_UpdateScale = true;
}

void CTransform::SetRelativeScale(const Vector2& Scale)
{
	SetRelativeScale(Vector3(Scale.x, Scale.y, m_RelativeScale.z));
}

void CTransform::SetRelativeScale(float x, float y)
{
	SetRelativeScale(Vector3(x, y, m_RelativeScale.z));
}

void CTransform::SetRelativeScale(float x, float y, float z)
{
	SetRelativeScale(Vector3(x, y, z));
}

void CTransform::SetRelativeScaleX(float x)
{
	SetRelativeScale(Vector3(x, m_RelativeScale.y, m_RelativeScale.z));
}

void CTransform::SetRelativeScaleY(float y)
{
	SetRelativeScale(Vector3(m_RelativeScale.x, y, m_RelativeScale.z));
}

void CTransform::SetRelativeScaleZ(float z)
{
	SetRelativeScale(Vector3(m_RelativeScale.x, m_RelativeScale.y, z));
}

void CTransform::SetRelativeRotation(const Vector3& Rot)
{
	m_RelativeRot = Rot;

	// 부모가 없을 경우라면 월드공간에서의 회전을 상대적인 회전과 동일한 회전으로 적용을 한다.
	if (!m_Parent)
	{
		m_WorldRot = Rot;
	}

	else
	{
		if (m_InheritRotX)
		{
			m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
		}

		else
		{
			m_WorldRot.x = m_RelativeRot.x;
		}

		if (m_InheritRotY)
		{
			m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
		}

		else
		{
			m_WorldRot.y = m_RelativeRot.y;
		}

		if (m_InheritRotZ)
		{
			m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
		}

		else
		{
			m_WorldRot.z = m_RelativeRot.z;
		}
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildRelativeRotation(Rot);
	}

	InheritRotation(true);
}

void CTransform::SetRelativeRotation(const Vector2& Rot)
{
	SetRelativeRotation(Vector3(Rot.x, Rot.y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotation(float x, float y)
{
	SetRelativeRotation(Vector3(x, y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotation(float x, float y, float z)
{
	SetRelativeRotation(Vector3(x, y, z));
}

void CTransform::SetRelativeRotationX(float x)
{
	SetRelativeRotation(Vector3(x, m_RelativeRot.y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotationY(float y)
{
	SetRelativeRotation(Vector3(m_RelativeRot.x, y, m_RelativeRot.z));
}

void CTransform::SetRelativeRotationZ(float z)
{
	SetRelativeRotation(Vector3(m_RelativeRot.x, m_RelativeRot.y, z));
}

void CTransform::SetRelativePosition(const Vector3& Pos)
{
	m_RelativePos = Pos;

	// 부모가 없을 경우라면 월드공간에서의 위치를 상대적인 위치와 동일한 위치로 적용을 한다.
	if (!m_Parent)
	{
		m_WorldPos = Pos + m_Offset;
	}

	else
	{
		m_WorldPos = Pos + m_Parent->GetWorldPos() + m_Offset;
	}

	InheritParentRotationPos();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildRelativePosition(Pos);
	}
}

void CTransform::SetRelativePosition(const Vector2& Pos)
{
	SetRelativePosition(Vector3(Pos.x, Pos.y, m_RelativePos.z));
}

void CTransform::SetRelativePosition(float x, float y)
{
	SetRelativePosition(Vector3(x, y, m_RelativePos.z));
}

void CTransform::SetRelativePosition(float x, float y, float z)
{
	SetRelativePosition(Vector3(x, y, z));
}

void CTransform::SetRelativePositionX(float x)
{
	SetRelativePosition(Vector3(x, m_RelativePos.y, m_RelativePos.z));
}

void CTransform::SetRelativePositionY(float y)
{
	SetRelativePosition(Vector3(m_RelativePos.x, y, m_RelativePos.z));
}

void CTransform::SetRelativePositionZ(float z)
{
	SetRelativePosition(Vector3(m_RelativePos.x, m_RelativePos.y, z));
}

void CTransform::AddRelativeScale(const Vector3& Scale)
{
	m_RelativeScale += Scale;

	// 부모가 없을 경우라면 월드공간에서의 크기는 상대적인 크기와 동일한 크기로 적용을 한다.
	if (!m_Parent)
	{
		m_WorldScale = m_RelativeScale;
	}

	else
	{
		if (m_InheritScale)
		{
			m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
		}

		else
		{
			m_WorldScale = m_RelativeScale;
		}
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildRelativeScale(Scale);
	}

	m_UpdateScale = true;
}

void CTransform::AddRelativeScale(const Vector2& Scale)
{
	AddRelativeScale(Vector3(Scale.x, Scale.y, 0.f));
}

void CTransform::AddRelativeScale(float x, float y)
{
	AddRelativeScale(Vector3(x, y, 0.f));
}

void CTransform::AddRelativeScale(float x, float y, float z)
{
	AddRelativeScale(Vector3(x, y, z));
}

void CTransform::AddRelativeScaleX(float x)
{
	AddRelativeScale(Vector3(x, 0.f, 0.f));
}

void CTransform::AddRelativeScaleY(float y)
{
	AddRelativeScale(Vector3(0.f, y, 0.f));
}

void CTransform::AddRelativeScaleZ(float z)
{
	AddRelativeScale(Vector3(0.f, 0.f, z));
}

void CTransform::AddRelativeRotation(const Vector3& Rot)
{
	m_RelativeRot += Rot;

	// 부모가 없을 경우라면 월드공간에서의 회전을 상대적인 회전과 동일한 회전으로 적용을 한다.
	if (!m_Parent)
	{
		m_WorldRot = m_RelativeRot;
	}

	else
	{
		if (m_InheritRotX)
		{
			m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
		}

		else
		{
			m_WorldRot.x = m_RelativeRot.x;
		}

		if (m_InheritRotY)
		{
			m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
		}

		else
		{
			m_WorldRot.y = m_RelativeRot.y;
		}

		if (m_InheritRotZ)
		{
			m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
		}

		else
		{
			m_WorldRot.z = m_RelativeRot.z;
		}
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildRelativeRotation(Rot);
	}

	InheritRotation(true);
}

void CTransform::AddRelativeRotation(const Vector2& Rot)
{
	AddRelativeRotation(Vector3(Rot.x, Rot.y, 0.f));
}

void CTransform::AddRelativeRotation(float x, float y)
{
	AddRelativeRotation(Vector3(x, y, 0.f));
}

void CTransform::AddRelativeRotation(float x, float y, float z)
{
	AddRelativeRotation(Vector3(x, y, z));
}

void CTransform::AddRelativeRotationX(float x)
{
	AddRelativeRotation(Vector3(x, 0.f, 0.f));
}

void CTransform::AddRelativeRotationY(float y)
{
	AddRelativeRotation(Vector3(0.f, y, 0.f));
}

void CTransform::AddRelativeRotationZ(float z)
{
	AddRelativeRotation(Vector3(0.f, 0.f, z));
}

void CTransform::AddRelativePosition(const Vector3& Pos)
{
	m_RelativePos += Pos;

	// 부모가 없을 경우라면 월드공간에서의 위치를 상대적인 위치와 동일한 위치로 적용을 한다.
	if (!m_Parent)
	{
		m_WorldPos = m_RelativePos + m_Offset;
	}

	else
	{
		m_WorldPos = m_RelativePos + m_Parent->GetWorldPos() + m_Offset;
	}

	InheritParentRotationPos();

	/*size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildRelativePosition(Pos);
	}*/
}

void CTransform::AddRelativePosition(const Vector2& Pos)
{
	AddRelativePosition(Vector3(Pos.x, Pos.y, 0.f));
}

void CTransform::AddRelativePosition(float x, float y)
{
	AddRelativePosition(Vector3(x, y, 0.f));
}

void CTransform::AddRelativePosition(float x, float y, float z)
{
	AddRelativePosition(Vector3(x, y, z));
}

void CTransform::AddRelativePositionX(float x)
{
	AddRelativePosition(Vector3(x, 0.f, 0.f));
}

void CTransform::AddRelativePositionY(float y)
{
	AddRelativePosition(Vector3(0.f, y, 0.f));
}

void CTransform::AddRelativePositionZ(float z)
{
	AddRelativePosition(Vector3(0.f, 0.f, z));
}

void CTransform::SetChildRelativeScale(const Vector3& Scale)
{
	//m_RelativeScale = Scale;

	if (m_InheritScale)
	{
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
	}

	else
	{
		m_WorldScale = m_RelativeScale;
	}

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildRelativeScale(Scale);
	}
}

void CTransform::SetChildRelativeRotation(const Vector3& Rot)
{
	if (m_InheritRotX)
	{
		//m_RelativeRot.x = Rot.x;
		m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
	}

	if (m_InheritRotY)
	{
		//m_RelativeRot.y = Rot.y;
		m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
	}

	if (m_InheritRotZ)
	{
		//m_RelativeRot.z = Rot.z;
		m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
	}

	Matrix matRot;
	Vector3 ConvertRot = m_WorldRot.ConvertAngle();

	DirectX::XMVECTOR Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildRelativeRotation(Rot);
	}
}

void CTransform::SetChildRelativePosition(const Vector3& Pos)
{
	//m_RelativePos = Pos;

	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos() + m_Offset;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildRelativePosition(Pos);
	}
}

void CTransform::AddChildRelativeScale(const Vector3& Scale)
{
	//m_RelativeScale += Scale;

	if (m_InheritScale)
	{
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
	}

	else
	{
		m_WorldScale = m_RelativeScale;
	}

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildRelativeScale(Scale);
	}
}

void CTransform::AddChildRelativeRotation(const Vector3& Rot)
{
	if (m_InheritRotX)
	{
		//m_RelativeRot.x += Rot.x;
		m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
	}

	if (m_InheritRotY)
	{
		//m_RelativeRot.y += Rot.y;
		m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
	}

	if (m_InheritRotZ)
	{
		//m_RelativeRot.z += Rot.z;
		m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
	}

	Matrix	matRot;
	Vector3	ConvertRot = m_WorldRot.ConvertAngle();

	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// 회전에 따라 축까지 모두 완료되었으면 자식노드를 반복하며 연산한다.
	m_UpdateRot = true;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildRelativeRotation(Rot);
	}
}

void CTransform::AddChildRelativePosition(const Vector3& Pos)
{
	//m_RelativePos += Pos;

	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos() + m_Offset;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildRelativePosition(Pos);
	}
}

void CTransform::SetWorldScale(const Vector3& Scale)
{
	m_WorldScale = Scale;
	
	// 부모가 없을 경우라면 상대적인 크기는 월드공간에서의 크기와 동일한 크기로 적용을 한다.
	if (!m_Parent)
		m_RelativeScale = m_WorldScale;

	else
	{
		if (m_InheritScale)
			m_RelativeScale = m_WorldScale / m_Parent->GetWorldScale();

		else
			m_RelativeScale = m_WorldScale;
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildWorldScale(Scale);
	}

	m_UpdateScale = true;
}

void CTransform::SetWorldScale(const Vector2& Scale)
{
	SetWorldScale(Vector3(Scale.x, Scale.y, m_WorldScale.z));
}

void CTransform::SetWorldScale(float x, float y)
{
	SetWorldScale(Vector3(x, y, m_WorldScale.z));
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	SetWorldScale(Vector3(x, y, z));
}

void CTransform::SetWorldScaleX(float x)
{
	SetWorldScale(Vector3(x, m_WorldScale.y, m_WorldScale.z));
}

void CTransform::SetWorldScaleY(float y)
{
	SetWorldScale(Vector3(m_WorldScale.x, y, m_WorldScale.z));
}

void CTransform::SetWorldScaleZ(float z)
{
	SetWorldScale(Vector3(m_WorldScale.x, m_WorldScale.y, z));
}

void CTransform::SetWorldRotation(const Vector3& Rot)
{
	m_WorldRot = Rot;
	
	// 부모가 없을 경우라면 상대적인 회전을 월드공간에서의 회전과 동일한 회전으로 적용을 한다.
	if (!m_Parent)
		m_RelativeRot = m_WorldRot;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildWorldRotation(Rot);
	}

	InheritWorldRotation(true);
}

void CTransform::SetWorldRotation(const Vector2& Rot)
{
	SetWorldRotation(Vector3(Rot.x, Rot.y, m_WorldRot.z));
}

void CTransform::SetWorldRotation(float x, float y)
{
	SetWorldRotation(Vector3(x, y, m_WorldRot.z));
}

void CTransform::SetWorldRotation(float x, float y, float z)
{
	SetWorldRotation(Vector3(x, y, z));
}

void CTransform::SetWorldRotationX(float x)
{
	SetWorldRotation(Vector3(x, m_WorldRot.y, m_WorldRot.z));
}

void CTransform::SetWorldRotationY(float y)
{
	SetWorldRotation(Vector3(m_WorldRot.x, y, m_WorldRot.z));
}

void CTransform::SetWorldRotationZ(float z)
{
	SetWorldRotation(Vector3(m_WorldRot.x, m_WorldRot.y, z));
}

void CTransform::SetWorldRotationAxis(const Vector3& OriginDir, const Vector3& View)
{
	m_UpdateRotAxis = true;

	Vector3	RotAxis = OriginDir.Cross(View);
	RotAxis.Normalize();

	float Angle = OriginDir.Angle(View);

	m_matRot = DirectX::XMMatrixRotationAxis(RotAxis.Convert(), DegreeToRadian(Angle));

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(m_matRot);
		m_WorldAxis[i].Normalize();
	}
}

void CTransform::SetWorldPosition(const Vector3& Pos)
{
	m_WorldPos = Pos;

	// 부모가 없을 경우라면 상대적인 위치를 월드공간에서의 위치와 동일한 위치로 적용을 한다.
	if (!m_Parent)
	{
		m_RelativePos = m_WorldPos - m_Offset;
	}

	InheritWorldParentRotationPos();

	/*size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildWorldPosition(Pos);
	}*/

}

void CTransform::SetWorldPosition(const Vector2& Pos)
{
	SetWorldPosition(Vector3(Pos.x, Pos.y, m_WorldPos.z));
}

void CTransform::SetWorldPosition(float x, float y)
{
	SetWorldPosition(Vector3(x, y, m_WorldPos.z));
}

void CTransform::SetWorldPosition(float x, float y, float z)
{
	SetWorldPosition(Vector3(x, y, z));
}

void CTransform::SetWorldPositionX(float x)
{
	SetWorldPosition(Vector3(x, m_WorldPos.y, m_WorldPos.z));
}

void CTransform::SetWorldPositionY(float y)
{
	SetWorldPosition(Vector3(m_WorldPos.x, y, m_WorldPos.z));
}

void CTransform::SetWorldPositionZ(float z)
{
	SetWorldPosition(Vector3(m_WorldPos.x, m_WorldPos.y, z));
}

void CTransform::SetOffset(const Vector3& Offset)
{
	m_Offset = Offset;
	m_WorldPos = m_RelativePos + Offset;

	InheritWorldParentRotationPos();

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetOffset(Offset);
	}
}

void CTransform::SetOffset(const Vector2& Offset)
{
	SetOffset(Vector3(Offset.x, Offset.y, m_Offset.z));
}

void CTransform::SetOffsetX(float x)
{
	SetOffset(Vector3(x, m_Offset.y, m_Offset.z));
}

void CTransform::SetOffsetY(float y)
{
	SetOffset(Vector3(m_Offset.x, y, m_Offset.z));
}

void CTransform::SetOffsetZ(float z)
{
	SetOffset(Vector3(m_Offset.x, m_Offset.y, z));
}

void CTransform::AddWorldScale(const Vector3& Scale)
{
	m_WorldScale += Scale;

	// 부모가 없을 경우라면 상대적인 크기는 월드공간에서의 크기와 동일한 크기로 적용을 한다.
	if (!m_Parent)
		m_RelativeScale = m_WorldScale;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildWorldScale(Scale);
	}

	m_UpdateScale = true;
}

void CTransform::AddWorldScale(const Vector2& Scale)
{
	AddWorldScale(Vector3(Scale.x, Scale.y, 0.f));
}

void CTransform::AddWorldScale(float x, float y)
{
	AddWorldScale(Vector3(x, y, 0.f));
}

void CTransform::AddWorldScale(float x, float y, float z)
{
	AddWorldScale(Vector3(x, y, z));
}

void CTransform::AddWorldScaleX(float x)
{
	AddWorldScale(Vector3(x, 0.f, 0.f));
}

void CTransform::AddWorldScaleY(float y)
{
	AddWorldScale(Vector3(0.f, y, 0.f));
}

void CTransform::AddWorldScaleZ(float z)
{
	AddWorldScale(Vector3(0.f, 0.f, z));
}

void CTransform::AddWorldRotation(const Vector3& Rot)
{
	m_WorldRot += Rot;

	// 부모가 없을 경우라면 상대적인 회전을 월드공간에서의 회전과 동일한 회전으로 적용을 한다.
	if (!m_Parent)
		m_RelativeRot = m_WorldRot;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildWorldRotation(Rot);
	}

	InheritWorldRotation(true);
}

void CTransform::AddWorldRotation(const Vector2& Rot)
{
	AddWorldRotation(Vector3(Rot.x, Rot.y, 0.f));
}

void CTransform::AddWorldRotation(float x, float y)
{
	AddWorldRotation(Vector3(x, y, 0.f));
}

void CTransform::AddWorldRotation(float x, float y, float z)
{
	AddWorldRotation(Vector3(x, y, z));
}

void CTransform::AddWorldRotationX(float x)
{
	AddWorldRotation(Vector3(x, 0.f, 0.f));
}

void CTransform::AddWorldRotationY(float y)
{
	AddWorldRotation(Vector3(0.f, y, 0.f));
}

void CTransform::AddWorldRotationZ(float z)
{
	AddWorldRotation(Vector3(0.f, 0.f, z));
}

void CTransform::AddWorldPosition(const Vector3& Pos)
{
	m_WorldPos += Pos;

	// 부모가 없을 경우라면 상대적인 위치를 월드공간에서의 위치와 동일한 위치로 적용을 한다.
	if (!m_Parent)
		m_RelativePos = m_WorldPos - m_Offset;

	InheritWorldParentRotationPos();

	/*size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildWorldPosition(Pos);
	}*/
}

void CTransform::AddWorldPosition(const Vector2& Pos)
{
	AddWorldPosition(Vector3(Pos.x, Pos.y, 0.f));
}

void CTransform::AddWorldPosition(float x, float y)
{
	AddWorldPosition(Vector3(x, y, 0.f));
}

void CTransform::AddWorldPosition(float x, float y, float z)
{
	AddWorldPosition(Vector3(x, y, z));
}

void CTransform::AddWorldPositionX(float x)
{
	AddWorldPosition(Vector3(x, 0.f, 0.f));
}

void CTransform::AddWorldPositionY(float y)
{
	AddWorldPosition(Vector3(0.f, y, 0.f));
}

void CTransform::AddWorldPositionZ(float z)
{
	AddWorldPosition(Vector3(0.f, 0.f, z));
}

void CTransform::AddOffset(const Vector3& Offset)
{
	m_Offset += Offset;
	m_WorldPos = m_RelativePos + m_Offset;

	InheritWorldParentRotationPos();

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddOffset(Offset);
	}
}

void CTransform::AddOffset(const Vector2& Offset)
{
	AddOffset(Vector3(Offset.x, Offset.y, 0.f));
}

void CTransform::AddOffsetX(float x)
{
	AddOffset(Vector3(x, 0.f, 0.f));
}

void CTransform::AddOffsetY(float y)
{
	AddOffset(Vector3(0.f, y, 0.f));
}

void CTransform::AddOffsetZ(float z)
{
	AddOffset(Vector3(0.f, 0.f, z));
}

void CTransform::SetChildWorldScale(const Vector3& Scale)
{
	//m_WorldScale = Scale;

	if (m_InheritScale)
	{
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();
	}

	else
	{
		m_WorldScale = m_RelativeScale;
	}

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildWorldScale(Scale);
	}

	m_UpdateScale = true;
}

void CTransform::SetChildWorldRotation(const Vector3& Rot)
{
	if (m_InheritRotX)
	{
		//m_WorldRot.x = Rot.x;
		m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;
	}

	if (m_InheritRotY)
	{
		//m_WorldRot.y = Rot.y;
		m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;
	}

	if (m_InheritRotZ)
	{
		//m_WorldRot.z = Rot.z;
		m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;
	}

	Vector3	ConvertRot = m_RelativeRot.ConvertAngle();

	DirectX::XMVECTOR Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertRot = m_WorldRot.ConvertAngle();

	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	m_UpdateRot = true;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildWorldRotation(Rot);
	}
}

void CTransform::SetChildWorldPosition(const Vector3& Pos)
{
	//m_WorldPos = Pos;

	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->SetChildWorldPosition(Pos);
	}
}

void CTransform::AddChildWorldScale(const Vector3& Scale)
{
	m_WorldScale += Scale;

	if (m_InheritScale)
	{
		m_RelativeScale = m_WorldScale / m_Parent->GetWorldScale();
	}

	else
	{
		m_RelativeScale = m_WorldScale;
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildWorldScale(Scale);
	}

	m_UpdateScale = true;
}

void CTransform::AddChildWorldRotation(const Vector3& Rot)
{
	if (m_InheritRotX)
	{
		m_WorldRot.x += Rot.x;
		m_RelativeRot.x = m_WorldRot.x - m_Parent->GetWorldRot().x;
	}

	if (m_InheritRotY)
	{
		m_WorldRot.y += Rot.y;
		m_RelativeRot.y = m_WorldRot.y - m_Parent->GetWorldRot().y;
	}

	if (m_InheritRotZ)
	{
		m_WorldRot.z += Rot.z;
		m_RelativeRot.z = m_WorldRot.z - m_Parent->GetWorldRot().z;
	}

	Vector3	ConvertRot = m_RelativeRot.ConvertAngle();

	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertRot = m_WorldRot.ConvertAngle();

	// 사원수를 구한다.
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x, ConvertRot.y, ConvertRot.z);

	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; i++)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	m_UpdateRot = true;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildWorldRotation(Rot);
	}
}

void CTransform::AddChildWorldPosition(const Vector3& Pos)
{
	m_WorldPos += Pos;

	m_RelativePos = m_WorldPos - m_Parent->GetWorldPos();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->AddChildWorldPosition(Pos);
	}
}

void CTransform::Start()
{
	InheritRotation(true);
	InheritParentRotationPos();
}

bool CTransform::Init()
{
	return true;
}

void CTransform::Update(float DeltaTime)
{
	if (m_Is2D)
	{
		float	y = m_WorldPos.y - m_Pivot.y * m_WorldScale.y;

		if (m_MinY > y)
			m_MinY = y;

		if (m_MaxY < y)
			m_MaxY = y;
	}
}

void CTransform::PostUpdate(float DeltaTime)
{
	if (m_Socket)
	{
		m_WorldScale *= m_Socket->GetMoveScale();
		m_WorldRot += m_Socket->GetMoveRot();
		m_WorldPos += m_Socket->GetMovePos();

		m_UpdateScale = true;
		m_UpdatePos = true;
		m_UpdateRot = true;
	}

	Vector3	WorldPos = m_WorldPos;

	if (m_Is2D)
	{
		float	SizeY = (m_MaxY + 1.f) - (m_MinY - 1.f);

		float	y = WorldPos.y - m_Pivot.y * m_WorldScale.y;

		float	z = (y - (m_MinY - 1.f)) / (SizeY / 1000.f);

		if (z != m_2DZ)
		{
			WorldPos.z = z;
			m_2DZ = z;
			m_UpdatePos = true;
		}
	}

	if (m_UpdateScale)
		m_matScale.Scaling(m_WorldScale);

	// x, y, z 축으로 회전을 하거나 아니면 임의의 축을 이용해서 회전을 하거나 둘중 하나의 경우만
	// 사용을 한다.
	if (m_UpdateRot && !m_UpdateRotAxis)
		m_matRot.Rotation(m_WorldRot);

	if (m_UpdatePos)
		m_matPos.Translation(WorldPos);

	// 월드 행렬은 위치, 크기, 회전중 하나라도 변화가 있어야 갱신한다.
	// 변화가 없을 경우 계산을 안한다.
	// 월드행렬 = 크기 * 자전 * 이동 * 공전 * 부모
	if (m_UpdateScale || m_UpdateRot || m_UpdateRotAxis || m_UpdatePos)
		m_matWorld = m_matScale * m_matRot * m_matPos;

	m_UpdateScale = false;
	m_UpdateRot = false;
	m_UpdateRotAxis = false;
	m_UpdatePos = false;
}

CTransform* CTransform::Clone() const
{
	return new CTransform(*this);
}

void CTransform::SetTransform()
{
	m_CBuffer->SetWorldMatrix(m_matWorld);

	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetProjMatrix();

	m_CBuffer->SetViewMatrix(matView);
	m_CBuffer->SetProjMatrix(matProj);

	m_CBuffer->SetPivot(m_Pivot);
	m_CBuffer->SetMeshSize(m_MeshSize);

	m_CBuffer->UpdateBuffer();

	Clear();
}

void CTransform::ComputeWorld()
{
	m_matWorld = m_matScale * m_matRot * m_matPos;
}

void CTransform::Save(FILE* File)
{
	fwrite(&m_Is2D, 1, 1, File);

	fwrite(&m_2DZ, sizeof(float), 1, File);

	fwrite(&m_InheritScale, 1, 1, File);
	fwrite(&m_InheritRotX, 1, 1, File);
	fwrite(&m_InheritRotY, 1, 1, File);
	fwrite(&m_InheritRotZ, 1, 1, File);
	fwrite(&m_InheritParentRotationPosX, 1, 1, File);
	fwrite(&m_InheritParentRotationPosY, 1, 1, File);
	fwrite(&m_InheritParentRotationPosZ, 1, 1, File);

	fwrite(&m_RelativeScale, sizeof(Vector3), 1, File);
	fwrite(&m_RelativeRot, sizeof(Vector3), 1, File);
	fwrite(&m_RelativePos, sizeof(Vector3), 1, File);
	fwrite(m_RelativeAxis, sizeof(Vector3), AXIS_MAX, File);

	fwrite(&m_WorldScale, sizeof(Vector3), 1, File);
	fwrite(&m_WorldRot, sizeof(Vector3), 1, File);
	fwrite(&m_WorldPos, sizeof(Vector3), 1, File);
	fwrite(m_WorldAxis, sizeof(Vector3), AXIS_MAX, File);

	fwrite(&m_OriginWorldScale, sizeof(Vector3), 1, File);
	fwrite(&m_OriginWorldRot, sizeof(Vector3), 1, File);
	fwrite(&m_OriginWorldPos, sizeof(Vector3), 1, File);

	fwrite(&m_Pivot, sizeof(Vector3), 1, File);
	fwrite(&m_MeshSize, sizeof(Vector3), 1, File);
}

void CTransform::Load(FILE* File)
{
	fread(&m_Is2D, 1, 1, File);

	fread(&m_2DZ, sizeof(float), 1, File);

	fread(&m_InheritScale, 1, 1, File);
	fread(&m_InheritRotX, 1, 1, File);
	fread(&m_InheritRotY, 1, 1, File);
	fread(&m_InheritRotZ, 1, 1, File);
	fread(&m_InheritParentRotationPosX, 1, 1, File);
	fread(&m_InheritParentRotationPosY, 1, 1, File);
	fread(&m_InheritParentRotationPosZ, 1, 1, File);

	fread(&m_RelativeScale, sizeof(Vector3), 1, File);
	fread(&m_RelativeRot, sizeof(Vector3), 1, File);
	fread(&m_RelativePos, sizeof(Vector3), 1, File);
	fread(m_RelativeAxis, sizeof(Vector3), AXIS_MAX, File);

	fread(&m_WorldScale, sizeof(Vector3), 1, File);
	fread(&m_WorldRot, sizeof(Vector3), 1, File);
	fread(&m_WorldPos, sizeof(Vector3), 1, File);
	fread(m_WorldAxis, sizeof(Vector3), AXIS_MAX, File);

	fread(&m_OriginWorldScale, sizeof(Vector3), 1, File);
	fread(&m_OriginWorldRot, sizeof(Vector3), 1, File);
	fread(&m_OriginWorldPos, sizeof(Vector3), 1, File);

	fread(&m_Pivot, sizeof(Vector3), 1, File);
	fread(&m_MeshSize, sizeof(Vector3), 1, File);

	m_UpdateScale = true;
	m_UpdateRot = true;
	m_UpdatePos = true;
}
