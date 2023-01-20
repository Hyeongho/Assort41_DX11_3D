
#include "TargetArm.h"
#include "../Input.h"

CTargetArm::CTargetArm() :
	m_TargetDistance(300.f),
	m_TargetDistanceAxis(AXIS::AXIS_Z),
	m_WheelZoomInOutEnable(true),
	m_WheelSensitivity(1.f),
	m_WheelZoomMin(50.f),
	m_WheelZoomMax(5000.f),
	m_WheelTickMove(30.f)
{
	SetTypeID<CTargetArm>();

	m_ComponentTypeName = "TargetArm";
}

CTargetArm::CTargetArm(const CTargetArm& component) :
	CSceneComponent(component)
{
	m_TargetDistance = component.m_TargetDistance;
	m_TargetOffset = component.m_TargetOffset;
	m_TargetDistanceAxis = component.m_TargetDistanceAxis;
	m_WheelZoomInOutEnable = component.m_WheelZoomInOutEnable;
	m_WheelSensitivity = component.m_WheelSensitivity;
	m_WheelZoomMin = component.m_WheelZoomMin;
	m_WheelZoomMax = component.m_WheelZoomMax;
	m_WheelTickMove = component.m_WheelTickMove;
}

CTargetArm::~CTargetArm()
{
}

void CTargetArm::Destroy()
{
	CSceneComponent::Destroy();
}

void CTargetArm::Start()
{
	CSceneComponent::Start();

	SetRelativePosition(m_TargetOffset);

	auto iter = m_vecChild.begin();
	auto iterEnd = m_vecChild.end();

	for ( ; iter != iterEnd; iter++)
	{
		(*iter)->SetRelativePosition(GetWorldAxis(m_TargetDistanceAxis) * -1.f * m_TargetDistance);
	}
}

bool CTargetArm::Init()
{
	CSceneComponent::Init();

	return true;
}

void CTargetArm::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_WheelZoomInOutEnable)
	{
		short Wheel = CInput::GetInst()->GetMouseWheel();

		if (Wheel != 0)
		{
			float Move = m_WheelSensitivity * m_WheelTickMove * Wheel * -1.f;

			m_TargetDistance += Move;

			if (m_TargetDistance < m_WheelZoomMin)
			{
				m_TargetDistance = m_WheelZoomMin;
			}

			else if (m_TargetDistance > m_WheelZoomMax)
			{
				m_TargetDistance = m_WheelZoomMax;
			}
		}
	}

	if (m_Parent)
	{
		/*auto iter = m_vecChild.begin();
		auto iterEnd = m_vecChild.end();

		for (; iter != iterEnd; iter++)
		{
			(*iter)->SetOffset(GetWorldAxis(m_TargetDistanceAxis) * -1.f * m_TargetDistance + m_TargetOffset);
		}*/

		/*Vector3	ParentPos = m_Parent->GetWorldPos();

		Vector3 Pos = ParentPos - GetWorldAxis(m_TargetDistanceAxis) * m_TargetDistance;

		SetWorldPosition(Pos + m_TargetOffset);*/
	}
}

void CTargetArm::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CTargetArm::Render()
{
	CSceneComponent::Render();
}

CTargetArm* CTargetArm::Clone() const
{
	return new CTargetArm(*this);
}

void CTargetArm::Save(FILE* File)
{
	CSceneComponent::Save(File);

	fwrite(&m_TargetOffset, sizeof(Vector3), 1, File);
	fwrite(&m_TargetDistance, sizeof(float), 1, File);
	fwrite(&m_TargetDistanceAxis, sizeof(AXIS), 1, File);
}

void CTargetArm::Load(FILE* File)
{
	CSceneComponent::Load(File);

	fread(&m_TargetOffset, sizeof(Vector3), 1, File);
	fread(&m_TargetDistance, sizeof(float), 1, File);
	fread(&m_TargetDistanceAxis, sizeof(AXIS), 1, File);
}
