
#include "CameraComponent.h"
#include "../Device.h"

CCameraComponent::CCameraComponent()	:
	m_CameraViewDistance(50000.f),
	m_CameraType(ECameraType::Camera3D)
{
	SetTypeID<CCameraComponent>();

	m_ComponentTypeName = "CameraComponent";
}

CCameraComponent::CCameraComponent(const CCameraComponent& component)	:
	CSceneComponent(component)
{
	m_CameraViewDistance = component.m_CameraViewDistance;
	m_matView = component.m_matView;
	m_matProj = component.m_matProj;
	m_CameraType = component.m_CameraType;

	ComputeProjectionMatrix();
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::ComputeProjectionMatrix()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	switch (m_CameraType)
	{
	case ECameraType::Camera2D:
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)RS.Width, 0.f,
			(float)RS.Height, 0.f, m_CameraViewDistance);
		break;
	case ECameraType::Camera3D:
		m_matProj = DirectX::XMMatrixPerspectiveFovLH(DegreeToRadian(90.f),
			(float)RS.Width / (float)RS.Height, 5.f, m_CameraViewDistance);
		break;
	case ECameraType::CameraUI:
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)RS.Width, 0.f,
			(float)RS.Height, 0.f, m_CameraViewDistance);
		break;
	}
}

void CCameraComponent::Destroy()
{
	CSceneComponent::Destroy();
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();

	ComputeProjectionMatrix();
}

bool CCameraComponent::Init()
{
	CSceneComponent::Init();

	return true;
}

void CCameraComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	/*
	ī�޶� �佺���̽� ��ȯ : ������� ��� ��ü�� ī�޶� 0, 0, 0���� �̵��ϴ� ��ŭ
	�̵���Ű�� ī�޶� �ٶ󺸴� ����(Z��)�� Z+ ������ �ٶ� �� �ְ� ȸ����Ű�� ��ŭ �̵���
	��� ��ü���� ���� �������� �׸�ŭ ���������ش�.

	�׷��� ī�޶�� ��������� ��ȯ�� �Ͼ ��� ����������� ī�޶��� x, y, z ����
	X : 1, 0, 0
	Y : 0, 1, 0
	Z : 0, 0, 1
	�� �ɰ��̴�.

	ī�޶��� ������ : WX, WY, WZ ���̶�� �Ѵ�.
	WX * ����� = 1, 0, 0
	WY * ����� = 0, 1, 0
	WZ * ����� = 0, 0, 1

	WXX WXY WXZ   11 12 13   1 0 0
	WYX WYY WYZ * 21 22 23 = 0 1 0
	WZX WZY WZZ   31 32 33   0 0 1

	�������� �̿��Ͽ� ������ ����� ������� �����ָ� �׵������ ���´�.
	������� �������� �̿��Ͽ� ������ ����� ������� �ȴٴ� ���̴�.

	�������� �̿��Ͽ� ������ ����� �������̱� ������ ���� �������Ѵ�.
	�̷��� ���� �������ϴ� ���� �̿��Ͽ� ������ ����� ��������̶�� �Ѵ�.

	��������� Ư¡ : ����İ� ��ġ����� ����.
	������� �������� �̿��Ͽ� ������ ����� ��ġ��İ� ���ٴ� �ǹ��̴�.
	WXX WXY WXZ    WXX WYX WZX
	WYX WYY WYZ => WXY WYY WZY
	WZX WZY WZZ    WXZ WYZ WZZ

	ī�޶��� ��ġ : PX, PY, PZ

	�̵����
	1    0   0  0
	0    1   0  0
	0    0   1  0
	-PX -PY -PZ 0

	ȸ�����
	WXX WYX WZX 0
	WXY WYY WZY 0
	WXZ WYZ WZZ 0
	0   0   0   1

	1    0   0  0   WXX WYX WZX 0   WXX   WYX    WZX  0
	0    1   0  0 * WXY WYY WZY 0 = WXY   WYY    WZY  0
	0    0   1  0	WXZ WYZ WZZ 0   WXZ   WYZ    WZZ  0
	-PX -PY -PZ 0	0   0   0   1   -P.WX -P.WY -P.WZ 1
	*/
	if (m_CameraType != ECameraType::CameraUI)
	{
		m_matView.Identity();

		/*
		WXX WXY WXZ 0
		WYX WYY WYZ 0
		WZX WZY WZZ 0
		0   0   0   1

		��ġ
		WXX WYX WZX 0
		WXY WYY WZY 0
		WXZ WYZ WZZ 0
		0   0   0   1
		*/
		for (int i = 0; i < 3; ++i)
		{
			Vector3	Axis = GetWorldAxis((AXIS)i);
			memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
		}

		m_matView.Transpose();

		Vector3	Pos = GetWorldPos();

		for (int i = 0; i < 3; ++i)
		{
			m_matView[3][i] = -Pos.Dot(GetWorldAxis((AXIS)i));
		}
	}
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

CCameraComponent* CCameraComponent::Clone() const
{
	return new CCameraComponent(*this);
}

void CCameraComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	fwrite(&m_CameraType, sizeof(ECameraType), 1, File);
	fwrite(&m_CameraViewDistance, sizeof(float), 1, File);

	fwrite(&m_matView, sizeof(Matrix), 1, File);
	fwrite(&m_matProj, sizeof(Matrix), 1, File);
}

void CCameraComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	fread(&m_CameraType, sizeof(ECameraType), 1, File);
	fread(&m_CameraViewDistance, sizeof(float), 1, File);

	fread(&m_matView, sizeof(Matrix), 1, File);
	fread(&m_matProj, sizeof(Matrix), 1, File);
}
