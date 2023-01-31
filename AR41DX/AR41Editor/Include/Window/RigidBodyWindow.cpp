#include "RigidBodyWindow.h"
#include "Engine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorListBox.h"
#include "Component/RigidBody.h"

CRigidBodyWindow::CRigidBodyWindow()
	: m_Gravity(nullptr)
	, m_Ground(nullptr)
	, m_Mass(nullptr)
	, m_FricCoeff(nullptr)
	, m_Force{}
	, m_Accel{}
	, m_AccelA{}
	, m_Velocity{}
	, m_MaxVelocity{}
	, m_MinVelocity{}
{
}

CRigidBodyWindow::~CRigidBodyWindow()
{
}

void CRigidBodyWindow::SetSelectComponent(CRigidBody* component)
{
	m_SelectRigidBody = component;
	m_Gravity->SetCheck(m_SelectRigidBody->GetGravity());
	m_Ground->SetCheck(m_SelectRigidBody->GetGround());
	m_Mass->SetFloat(m_SelectRigidBody->GetMass());
	m_FricCoeff->SetFloat(m_SelectRigidBody->GetFricCoeff());
	m_Force[0]->SetFloat(m_SelectRigidBody->GetForce().x);
	m_Force[1]->SetFloat(m_SelectRigidBody->GetForce().y);
	m_Force[2]->SetFloat(m_SelectRigidBody->GetForce().z);
	m_Accel[0]->SetFloat(m_SelectRigidBody->GetAccel().x);
	m_Accel[1]->SetFloat(m_SelectRigidBody->GetAccel().y);
	m_Accel[2]->SetFloat(m_SelectRigidBody->GetAccel().z);
	m_AccelA[0]->SetFloat(m_SelectRigidBody->GetAccelAlpha().x);
	m_AccelA[1]->SetFloat(m_SelectRigidBody->GetAccelAlpha().y);
	m_AccelA[2]->SetFloat(m_SelectRigidBody->GetAccelAlpha().z);
	m_Velocity[0]->SetFloat(m_SelectRigidBody->GetVelocity().x);
	m_Velocity[1]->SetFloat(m_SelectRigidBody->GetVelocity().y);
	m_Velocity[2]->SetFloat(m_SelectRigidBody->GetVelocity().z);
	m_MaxVelocity[0]->SetFloat(m_SelectRigidBody->GetMaxVelocity().x);
	m_MaxVelocity[1]->SetFloat(m_SelectRigidBody->GetMaxVelocity().y);
	m_MaxVelocity[2]->SetFloat(m_SelectRigidBody->GetMaxVelocity().z);
	m_MinVelocity[0]->SetFloat(m_SelectRigidBody->GetMinVelocity().x);
	m_MinVelocity[1]->SetFloat(m_SelectRigidBody->GetMinVelocity().y);
	m_MinVelocity[2]->SetFloat(m_SelectRigidBody->GetMinVelocity().z);
}

bool CRigidBodyWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("리기드 바디");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("line");
	CEditorButton* button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetColor(29, 47, 73, 255);
	button->SetClickCallback<CRigidBodyWindow>(this, &CRigidBodyWindow::RigidBodyChangeCallback);
	m_Gravity = CreateWidget<CEditorCheckBox>("중력");
	line = CreateWidget<CEditorSameLine>("line");
	m_Ground = CreateWidget<CEditorCheckBox>("지형");

	label = CreateWidget<CEditorLabel>("힘");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	m_Force[0] = CreateWidget<CEditorInput>("ForceX", 80.f, 30.f);
	m_Force[0]->SetHideName("ForceX");
	m_Force[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Force[1] = CreateWidget<CEditorInput>("ForceY", 80.f, 30.f);
	m_Force[1]->SetHideName("ForceY");
	m_Force[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Force[2] = CreateWidget<CEditorInput>("ForceZ", 80.f, 30.f);
	m_Force[2]->SetHideName("ForceZ");
	m_Force[2]->SetInputType(EImGuiInputType::Float);

	label = CreateWidget<CEditorLabel>("가속도");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	m_Accel[0] = CreateWidget<CEditorInput>("AccelX", 80.f, 30.f);
	m_Accel[0]->SetHideName("AccelX");
	m_Accel[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Accel[1] = CreateWidget<CEditorInput>("AccelY", 80.f, 30.f);
	m_Accel[1]->SetHideName("AccelY");
	m_Accel[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Accel[2] = CreateWidget<CEditorInput>("AccelZ", 80.f, 30.f);
	m_Accel[2]->SetHideName("AccelZ");
	m_Accel[2]->SetInputType(EImGuiInputType::Float);

	label = CreateWidget<CEditorLabel>("추가 가속도");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	m_AccelA[0] = CreateWidget<CEditorInput>("AccelAX", 80.f, 30.f);
	m_AccelA[0]->SetHideName("AccelAX");
	m_AccelA[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_AccelA[1] = CreateWidget<CEditorInput>("AccelAY", 80.f, 30.f);
	m_AccelA[1]->SetHideName("AccelAY");
	m_AccelA[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_AccelA[2] = CreateWidget<CEditorInput>("AccelAZ", 80.f, 30.f);
	m_AccelA[2]->SetHideName("AccelAZ");
	m_AccelA[2]->SetInputType(EImGuiInputType::Float);

	label = CreateWidget<CEditorLabel>("속도");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	m_Velocity[0] = CreateWidget<CEditorInput>("VelocityX", 80.f, 30.f);
	m_Velocity[0]->SetHideName("VelocityX");
	m_Velocity[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Velocity[1] = CreateWidget<CEditorInput>("VelocityY", 80.f, 30.f);
	m_Velocity[1]->SetHideName("VelocityY");
	m_Velocity[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Velocity[2] = CreateWidget<CEditorInput>("VelocityZ", 80.f, 30.f);
	m_Velocity[2]->SetHideName("VelocityZ");
	m_Velocity[2]->SetInputType(EImGuiInputType::Float);

	label = CreateWidget<CEditorLabel>("최대속도");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	m_MaxVelocity[0] = CreateWidget<CEditorInput>("MaxVelocityX", 80.f, 30.f);
	m_MaxVelocity[0]->SetHideName("MaxVelocityX");
	m_MaxVelocity[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_MaxVelocity[1] = CreateWidget<CEditorInput>("MaxVelocityY", 80.f, 30.f);
	m_MaxVelocity[1]->SetHideName("MaxVelocityY");
	m_MaxVelocity[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_MaxVelocity[2] = CreateWidget<CEditorInput>("MaxVelocityZ", 80.f, 30.f);
	m_MaxVelocity[2]->SetHideName("MaxVelocityZ");
	m_MaxVelocity[2]->SetInputType(EImGuiInputType::Float);

	label = CreateWidget<CEditorLabel>("최소속도");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	m_MinVelocity[0] = CreateWidget<CEditorInput>("MinVelocityX", 80.f, 30.f);
	m_MinVelocity[0]->SetHideName("MinVelocityX");
	m_MinVelocity[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_MinVelocity[1] = CreateWidget<CEditorInput>("MinVelocityY", 80.f, 30.f);
	m_MinVelocity[1]->SetHideName("MinVelocityY");
	m_MinVelocity[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_MinVelocity[2] = CreateWidget<CEditorInput>("MinVelocityZ", 80.f, 30.f);
	m_MinVelocity[2]->SetHideName("MinVelocityZ");
	m_MinVelocity[2]->SetInputType(EImGuiInputType::Float);

	m_Mass = CreateWidget<CEditorInput>("질량", 80.f, 30.f);
	m_Mass->SetInputType(EImGuiInputType::Float);
	m_FricCoeff = CreateWidget<CEditorInput>("마찰계수", 80.f, 30.f);
	m_FricCoeff->SetInputType(EImGuiInputType::Float);
	return true;
}

void CRigidBodyWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectRigidBody)
	{
		if (!m_SelectRigidBody->GetActive())
		{
			m_SelectRigidBody = nullptr;
		}
	}
}

void CRigidBodyWindow::RigidBodyChangeCallback()
{
	if (!m_SelectRigidBody)
	{
		return;
	}
	m_SelectRigidBody->SetGravity(m_Gravity->GetCheck());
	m_SelectRigidBody->SetGround(m_Ground->GetCheck());
	m_SelectRigidBody->SetMass(m_Mass->GetFloat());
	m_SelectRigidBody->SetFricCoeff(m_FricCoeff->GetFloat());
	m_SelectRigidBody->SetForce(m_Force[0]->GetFloat(), m_Force[1]->GetFloat(), m_Force[2]->GetFloat());
	m_SelectRigidBody->SetAccel(m_Accel[0]->GetFloat(), m_Accel[1]->GetFloat(), m_Accel[2]->GetFloat());
	m_SelectRigidBody->SetAccelAlpha(m_AccelA[0]->GetFloat(), m_AccelA[1]->GetFloat(), m_AccelA[2]->GetFloat());
	m_SelectRigidBody->SetVelocity(m_Velocity[0]->GetFloat(), m_Velocity[1]->GetFloat(), m_Velocity[2]->GetFloat());
	m_SelectRigidBody->SetMaxVelocity(m_MaxVelocity[0]->GetFloat(), 
		m_MaxVelocity[1]->GetFloat(), m_MaxVelocity[2]->GetFloat());
	m_SelectRigidBody->SetMinVelocity(m_MinVelocity[0]->GetFloat(), 
		m_MinVelocity[1]->GetFloat(), m_MinVelocity[2]->GetFloat());
}