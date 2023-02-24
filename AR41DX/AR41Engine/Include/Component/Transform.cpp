
#include "Transform.h"
#include "../Resource/Shader/TransformConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"
#include "../Resource/Animation/SkeletonSocket.h"
#include "../Engine.h"

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
	for (int i = 0; i < AXIS_MAX; ++i)
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
	// �θ� ���� ��� �θ��� ȸ���� ������ �޴´�.
	// ��, x, y, z �� ȸ���� ��� ���� ������ ������ ������ ���� �� �ְ� �صξ����Ƿ�
	// �� �ึ�� ����Ѵ�.
	if (m_Parent)
	{
		// x, y, z �� ȸ���� �ϳ��� �θ� ������ �ް� ���� Ʈ�������� �ƴ� ȸ���� �Ͼ
		// Ʈ�������� �ڽ� Ʈ�������� ��� ��ġ�� ������ �Ǿ�� �Ѵ�.
		if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !Current)
			InheritParentRotationPos();
	}

	// x, y, z �� ȸ�� ������ �̿��Ͽ� ���� ȸ������� ���ϱ� ���� ������� ������.
	// Degree�� Radian���� ��ȯ�Ѵ�.
	Vector3	ConvertRot = m_RelativeRot.ConvertAngle();

	// ������� ���Ѵ�.
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	// ������� ������� ȸ������� �̿��ؼ� ������� ���� ȸ����Ų��.
	// ��������� �����Ҷ� 41, 42, 43 �κп� ��ġ�� ����.
	// ���� ��ġ�� �ʿ� ���� ���⸸ �����ϹǷ� ��ġ�� ������ �ȹް� ȸ������ ������ �ް�
	// �����ָ� �ȴ�. ��, x, y, z �� ���� ��� 4x4 ��İ� ���ϱ� ���� w�� 0���� ����ٸ�
	// ����� 41, 42, 43�� ������ �ȹް� �������� �ǹǷ� ��ġ�� ������ �ȹ������� w�� 0����,
	// ������ �������� w�� 1�� �ؼ� ���ϸ� �ȴ�.
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertRot = m_WorldRot.ConvertAngle();

	// ������� ���Ѵ�.
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// ȸ���� ���� ����� ��� �Ϸ�Ǿ����� �ڽĳ�带 �ݺ��ϸ� �����Ѵ�.
	m_UpdateRot = true;

	// ��� �ڽĳ�带 �����Ѵ�.
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->InheritRotation(false);
	}
}

void CTransform::InheritParentRotationPos()
{
	// �θ� ���� ��� �θ��� ��ġ�� ������ �ް� ȸ������ ������ �޾� ����Ѵ�.
	if (m_Parent)
	{
		Matrix	matRot;
		Vector3	ParentRot;

		if (m_InheritRotX)
			ParentRot.x = m_Parent->GetWorldRot().x;

		if (m_InheritRotY)
			ParentRot.y = m_Parent->GetWorldRot().y;

		if (m_InheritRotZ)
			ParentRot.z = m_Parent->GetWorldRot().z;

		if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
		{
			Vector3	ConvertRot = ParentRot.ConvertAngle();

			DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
				ConvertRot.y, ConvertRot.z);

			// ����� ���Ѵ�.
			Matrix	matRot;
			matRot.RotationQuaternion(Qut);

			// �θ��� ��ġ�� ����� 41, 42, 43 �� �����Ѵ�.
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

	// ��� �ڽĳ�带 �����Ѵ�.
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->InheritParentRotationPos();
	}
}

void CTransform::InheritWorldRotation(bool Current)
{
	// �θ� ���� ��� �θ��� ȸ���� ������ �޴´�.
	// ��, x, y, z �� ȸ���� ��� ���� ������ ������ ������ ���� �� �ְ� �صξ����Ƿ�
	// �� �ึ�� ����Ѵ�.
	if (m_Parent)
	{
		// x, y, z �� ȸ���� �ϳ��� �θ� ������ �ް� ���� Ʈ�������� �ƴ� ȸ���� �Ͼ
		// Ʈ�������� �ڽ� Ʈ�������� ��� ��ġ�� ������ �Ǿ�� �Ѵ�.
		if ((m_InheritRotX || m_InheritRotY || m_InheritRotZ) && !Current)
			InheritWorldParentRotationPos();
	}

	else
	{
		m_WorldRot = m_RelativeRot;
	}

	// x, y, z �� ȸ�� ������ �̿��Ͽ� ���� ȸ������� ���ϱ� ���� ������� ������.
	// Degree�� Radian���� ��ȯ�Ѵ�.
	Vector3	ConvertRot = m_RelativeRot.ConvertAngle();

	// ������� ���Ѵ�.
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	// ������� ������� ȸ������� �̿��ؼ� ������� ���� ȸ����Ų��.
	// ��������� �����Ҷ� 41, 42, 43 �κп� ��ġ�� ����.
	// ���� ��ġ�� �ʿ� ���� ���⸸ �����ϹǷ� ��ġ�� ������ �ȹް� ȸ������ ������ �ް�
	// �����ָ� �ȴ�. ��, x, y, z �� ���� ��� 4x4 ��İ� ���ϱ� ���� w�� 0���� ����ٸ�
	// ����� 41, 42, 43�� ������ �ȹް� �������� �ǹǷ� ��ġ�� ������ �ȹ������� w�� 0����,
	// ������ �������� w�� 1�� �ؼ� ���ϸ� �ȴ�.
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertRot = m_WorldRot.ConvertAngle();

	// ������� ���Ѵ�.
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// ȸ���� ���� ����� ��� �Ϸ�Ǿ����� �ڽĳ�带 �ݺ��ϸ� �����Ѵ�.
	m_UpdateRot = true;

	// ��� �ڽĳ�带 �����Ѵ�.
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->InheritWorldRotation(false);
	}
}

void CTransform::InheritWorldParentRotationPos()
{
	// �θ� ���� ��� �θ��� ��ġ�� ������ �ް� ȸ������ ������ �޾� ����Ѵ�.
	if (m_Parent)
	{
		Matrix	matRot;
		Vector3	ParentRot;

		if (m_Owner->GetName() == "RightChild")
			int a = 0;

		if (m_InheritRotX)
			ParentRot.x = m_Parent->GetWorldRot().x;

		if (m_InheritRotY)
			ParentRot.y = m_Parent->GetWorldRot().y;

		if (m_InheritRotZ)
			ParentRot.z = m_Parent->GetWorldRot().z;

		if (m_InheritRotX || m_InheritRotY || m_InheritRotZ)
		{
			// �θ�κ��� ȸ���� ��ġ�� ������ �Ǳ� ������ ������ġ�� ������ ��� �ش� ��ġ�κ��� 
			// ������ ������� ��ġ�� ���� �Ŀ� �θ� ȸ���� ��ȸ���� �̿��Ͽ� ���� �������
			// ��ġ�� �����־�� �Ѵ�.
			//Vector3	RelativePos = m_WorldPos - m_Parent->GetWorldPos();

			// ��ȸ��.

			Vector3	ConvertRot = ParentRot.ConvertAngle();

			DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
				ConvertRot.y, ConvertRot.z);

			// ����� ���Ѵ�.
			Matrix	matRot;
			matRot.RotationQuaternion(Qut);

			// �θ��� ��ġ�� ����� 41, 42, 43 �� �����Ѵ�.
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

	// ��� �ڽĳ�带 �����Ѵ�.
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->InheritParentRotationPos();
	}
}

void CTransform::SetRelativeScale(const Vector3& Scale)
{
	m_RelativeScale = Scale;

	// �θ� ���� ����� ������������� ũ��� ������� ũ��� ������ ũ��� ������ �Ѵ�.
	if (!m_Parent)
		m_WorldScale = Scale;

	else
	{
		if (m_InheritScale)
			m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();

		else
			m_WorldScale = m_RelativeScale;
	}

	Vector3	MeshSizeScale = m_MeshSize * m_WorldScale;

	m_Radius = MeshSizeScale.Length() * 0.5f;

	m_Center = (GetMin() + GetMax()) * 0.5f;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// �θ� ���� ����� ������������� ȸ���� ������� ȸ���� ������ ȸ������ ������ �Ѵ�.
	if (!m_Parent)
		m_WorldRot = Rot;

	else
	{
		if (m_InheritRotX)
			m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;

		else
			m_WorldRot.x = m_RelativeRot.x;

		if (m_InheritRotY)
			m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;

		else
			m_WorldRot.y = m_RelativeRot.y;

		if (m_InheritRotZ)
			m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;

		else
			m_WorldRot.z = m_RelativeRot.z;
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// �θ� ���� ����� ������������� ��ġ�� ������� ��ġ�� ������ ��ġ�� ������ �Ѵ�.
	if (!m_Parent)
		m_WorldPos = Pos + m_Offset;

	else
		m_WorldPos = Pos + m_Parent->GetWorldPos() + m_Offset;

	InheritParentRotationPos();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// �θ� ���� ����� ������������� ũ��� ������� ũ��� ������ ũ��� ������ �Ѵ�.
	if (!m_Parent)
		m_WorldScale = m_RelativeScale;

	else
	{
		if (m_InheritScale)
			m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();

		else
			m_WorldScale = m_RelativeScale;
	}

	Vector3	MeshSizeScale = m_MeshSize * m_WorldScale;

	m_Radius = MeshSizeScale.Length() * 0.5f;

	m_Center = (GetMin() + GetMax()) * 0.5f;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// �θ� ���� ����� ������������� ȸ���� ������� ȸ���� ������ ȸ������ ������ �Ѵ�.
	if (!m_Parent)
		m_WorldRot = m_RelativeRot;

	else
	{
		if (m_InheritRotX)
			m_WorldRot.x = m_RelativeRot.x + m_Parent->GetWorldRot().x;

		else
			m_WorldRot.x = m_RelativeRot.x;

		if (m_InheritRotY)
			m_WorldRot.y = m_RelativeRot.y + m_Parent->GetWorldRot().y;

		else
			m_WorldRot.y = m_RelativeRot.y;

		if (m_InheritRotZ)
			m_WorldRot.z = m_RelativeRot.z + m_Parent->GetWorldRot().z;

		else
			m_WorldRot.z = m_RelativeRot.z;
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// �θ� ���� ����� ������������� ��ġ�� ������� ��ġ�� ������ ��ġ�� ������ �Ѵ�.
	if (!m_Parent)
		m_WorldPos = m_RelativePos + m_Offset;

	else
		m_WorldPos = m_RelativePos + m_Parent->GetWorldPos() + m_Offset;

	InheritParentRotationPos();

	/*size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();

	else
		m_WorldScale = m_RelativeScale;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// ����� ���Ѵ�.
	Matrix	matRot;
	Vector3	ConvertRot = m_WorldRot.ConvertAngle();

	// ������� ���Ѵ�.
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// ȸ���� ���� ����� ��� �Ϸ�Ǿ����� �ڽĳ�带 �ݺ��ϸ� �����Ѵ�.
	m_UpdateRot = true;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetChildRelativeRotation(Rot);
	}
}

void CTransform::SetChildRelativePosition(const Vector3& Pos)
{
	//m_RelativePos = Pos;

	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos() + m_Offset;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetChildRelativePosition(Pos);
	}
}

void CTransform::AddChildRelativeScale(const Vector3& Scale)
{
	//m_RelativeScale += Scale;

	if (m_InheritScale)
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();

	else
		m_WorldScale = m_RelativeScale;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// ����� ���Ѵ�.
	Matrix	matRot;
	Vector3	ConvertRot = m_WorldRot.ConvertAngle();

	// ������� ���Ѵ�.
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// ȸ���� ���� ����� ��� �Ϸ�Ǿ����� �ڽĳ�带 �ݺ��ϸ� �����Ѵ�.
	m_UpdateRot = true;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->AddChildRelativeRotation(Rot);
	}
}

void CTransform::AddChildRelativePosition(const Vector3& Pos)
{
	//m_RelativePos += Pos;

	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos() + m_Offset;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->AddChildRelativePosition(Pos);
	}
}

float CTransform::GetRadius()	const
{
	if (CEngine::GetInst()->GetEditorMode())
	{
		if (m_Radius < 10.f)
			return 10.f;
	}

	return m_Radius;
}

void CTransform::SetWorldScale(const Vector3& Scale)
{
	m_WorldScale = Scale;
	
	// �θ� ���� ����� ������� ũ��� ������������� ũ��� ������ ũ��� ������ �Ѵ�.
	if (!m_Parent)
		m_RelativeScale = m_WorldScale;

	else
	{
		if (m_InheritScale)
			m_RelativeScale = m_WorldScale / m_Parent->GetWorldScale();

		else
			m_RelativeScale = m_WorldScale;
	}

	Vector3	MeshSizeScale = m_MeshSize * m_WorldScale;

	m_Radius = MeshSizeScale.Length() * 0.5f;

	m_Center = (GetMin() + GetMax()) * 0.5f;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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
	
	// �θ� ���� ����� ������� ȸ���� ������������� ȸ���� ������ ȸ������ ������ �Ѵ�.
	if (!m_Parent)
		m_RelativeRot = m_WorldRot;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

void CTransform::SetWorldRotationAxis(const Vector3& OriginDir, 
	const Vector3& View)
{
	m_UpdateRotAxis = true;

	Vector3	RotAxis = OriginDir.Cross(View);
	RotAxis.Normalize();

	float Angle = OriginDir.Angle(View);

	m_matRot = DirectX::XMMatrixRotationAxis(RotAxis.Convert(),
		DegreeToRadian(Angle));

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(m_matRot);
		m_WorldAxis[i].Normalize();
	}
}

void CTransform::SetWorldPosition(const Vector3& Pos)
{
	m_WorldPos = Pos;

	// �θ� ���� ����� ������� ��ġ�� ������������� ��ġ�� ������ ��ġ�� ������ �Ѵ�.
	if (!m_Parent)
		m_RelativePos = m_WorldPos - m_Offset;

	InheritWorldParentRotationPos();

	/*size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// �θ� ���� ����� ������� ũ��� ������������� ũ��� ������ ũ��� ������ �Ѵ�.
	if (!m_Parent)
		m_RelativeScale = m_WorldScale;

	Vector3	MeshSizeScale = m_MeshSize * m_WorldScale;

	m_Radius = MeshSizeScale.Length() * 0.5f;

	m_Center = (GetMin() + GetMax()) * 0.5f;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// �θ� ���� ����� ������� ȸ���� ������������� ȸ���� ������ ȸ������ ������ �Ѵ�.
	if (!m_Parent)
		m_RelativeRot = m_WorldRot;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// �θ� ���� ����� ������� ��ġ�� ������������� ��ġ�� ������ ��ġ�� ������ �Ѵ�.
	if (!m_Parent)
		m_RelativePos = m_WorldPos - m_Offset;

	InheritWorldParentRotationPos();

	/*size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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
		m_WorldScale = m_RelativeScale * m_Parent->GetWorldScale();

	else
		m_WorldScale = m_RelativeScale;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// x, y, z �� ȸ�� ������ �̿��Ͽ� ���� ȸ������� ���ϱ� ���� ������� ������.
	// Degree�� Radian���� ��ȯ�Ѵ�.
	Vector3	ConvertRot = m_RelativeRot.ConvertAngle();

	// ������� ���Ѵ�.
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	// ������� ������� ȸ������� �̿��ؼ� ������� ���� ȸ����Ų��.
	// ��������� �����Ҷ� 41, 42, 43 �κп� ��ġ�� ����.
	// ���� ��ġ�� �ʿ� ���� ���⸸ �����ϹǷ� ��ġ�� ������ �ȹް� ȸ������ ������ �ް�
	// �����ָ� �ȴ�. ��, x, y, z �� ���� ��� 4x4 ��İ� ���ϱ� ���� w�� 0���� ����ٸ�
	// ����� 41, 42, 43�� ������ �ȹް� �������� �ǹǷ� ��ġ�� ������ �ȹ������� w�� 0����,
	// ������ �������� w�� 1�� �ؼ� ���ϸ� �ȴ�.
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertRot = m_WorldRot.ConvertAngle();

	// ������� ���Ѵ�.
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// ȸ���� ���� ����� ��� �Ϸ�Ǿ����� �ڽĳ�带 �ݺ��ϸ� �����Ѵ�.
	m_UpdateRot = true;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetChildWorldRotation(Rot);
	}
}

void CTransform::SetChildWorldPosition(const Vector3& Pos)
{
	//m_WorldPos = Pos;

	m_WorldPos = m_RelativePos + m_Parent->GetWorldPos();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetChildWorldPosition(Pos);
	}
}

void CTransform::AddChildWorldScale(const Vector3& Scale)
{
	m_WorldScale += Scale;

	if (m_InheritScale)
		m_RelativeScale = m_WorldScale / m_Parent->GetWorldScale();

	else
		m_RelativeScale = m_WorldScale;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	// x, y, z �� ȸ�� ������ �̿��Ͽ� ���� ȸ������� ���ϱ� ���� ������� ������.
	// Degree�� Radian���� ��ȯ�Ѵ�.
	Vector3	ConvertRot = m_RelativeRot.ConvertAngle();

	// ������� ���Ѵ�.
	DirectX::XMVECTOR	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	Matrix	matRot;
	matRot.RotationQuaternion(Qut);

	// ������� ������� ȸ������� �̿��ؼ� ������� ���� ȸ����Ų��.
	// ��������� �����Ҷ� 41, 42, 43 �κп� ��ġ�� ����.
	// ���� ��ġ�� �ʿ� ���� ���⸸ �����ϹǷ� ��ġ�� ������ �ȹް� ȸ������ ������ �ް�
	// �����ָ� �ȴ�. ��, x, y, z �� ���� ��� 4x4 ��İ� ���ϱ� ���� w�� 0���� ����ٸ�
	// ����� 41, 42, 43�� ������ �ȹް� �������� �ǹǷ� ��ġ�� ������ �ȹ������� w�� 0����,
	// ������ �������� w�� 1�� �ؼ� ���ϸ� �ȴ�.
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_RelativeAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_RelativeAxis[i].Normalize();
	}

	ConvertRot = m_WorldRot.ConvertAngle();

	// ������� ���Ѵ�.
	Qut = DirectX::XMQuaternionRotationRollPitchYaw(ConvertRot.x,
		ConvertRot.y, ConvertRot.z);

	// ����� ���Ѵ�.
	matRot.RotationQuaternion(Qut);

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(matRot);
		m_WorldAxis[i].Normalize();
	}

	// ȸ���� ���� ����� ��� �Ϸ�Ǿ����� �ڽĳ�带 �ݺ��ϸ� �����Ѵ�.
	m_UpdateRot = true;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->AddChildWorldRotation(Rot);
	}
}

void CTransform::AddChildWorldPosition(const Vector3& Pos)
{
	m_WorldPos += Pos;

	m_RelativePos = m_WorldPos - m_Parent->GetWorldPos();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
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

	m_Center = (GetMin() + GetMax()) * 0.5f;
	m_Radius = (GetMax() - GetMin()).Length() * 0.5f;

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

	// x, y, z ������ ȸ���� �ϰų� �ƴϸ� ������ ���� �̿��ؼ� ȸ���� �ϰų� ���� �ϳ��� ��츸
	// ����� �Ѵ�.
	if (m_UpdateRot && !m_UpdateRotAxis)
		m_matRot.Rotation(m_WorldRot);

	if (m_UpdatePos)
		m_matPos.Translation(WorldPos);

	// ���� ����� ��ġ, ũ��, ȸ���� �ϳ��� ��ȭ�� �־�� �����Ѵ�.
	// ��ȭ�� ���� ��� ����� ���Ѵ�.
	// ������� = ũ�� * ���� * �̵� * ���� * �θ�
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

void CTransform::SetShadowMapTransform()
{
	m_CBuffer->SetWorldMatrix(m_matWorld);


	// ������ ��ġ���� �ٶ󺸴� �׸��� �ʿ� ������� �̿��Ѵ�.
	Matrix	matView = m_Scene->GetCameraManager()->GetCurrentCamera()->GetShadowViewMatrix();

	// �׸��� �ʿ� �ػ󵵰� ����Ǵ� ������ĸ� �̿��Ѵ�
	Matrix	matProj = m_Scene->GetCameraManager()->GetCurrentCamera()->GetShadowProjMatrix();

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
