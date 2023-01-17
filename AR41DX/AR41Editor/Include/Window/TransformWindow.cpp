
#include "TransformWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorTree.h"
#include "Component/SceneComponent.h"

CTransformWindow::CTransformWindow()
{
}

CTransformWindow::~CTransformWindow()
{
}

void CTransformWindow::SetSelectComponent(CSceneComponent* Component)
{
	m_SelectComponent = Component;
}

void CTransformWindow::SetPos(const Vector3& Pos)
{
	m_Pos[0]->SetFloat(Pos.x);
	m_Pos[1]->SetFloat(Pos.y);
	m_Pos[2]->SetFloat(Pos.z);
}

void CTransformWindow::SetRotation(const Vector3& Rot)
{
	m_Rot[0]->SetFloat(Rot.x);
	m_Rot[1]->SetFloat(Rot.y);
	m_Rot[2]->SetFloat(Rot.z);
}

void CTransformWindow::SetScale(const Vector3& Scale)
{
	m_Scale[0]->SetFloat(Scale.x);
	m_Scale[1]->SetFloat(Scale.y);
	m_Scale[2]->SetFloat(Scale.z);
}

bool CTransformWindow::Init()
{
	CEditorLabel* Label = CreateWidget<CEditorLabel>("위치");

	Label->SetColor(29, 47, 73, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSizeY(30.f);

	CEditorSameLine* Line = CreateWidget<CEditorSameLine>("Line");

	m_Pos[0] = CreateWidget<CEditorInput>("PosX", 80.f, 30.f);
	m_Pos[0]->SetHideName("PosX");
	m_Pos[0]->SetInputType(EImGuiInputType::Float);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_Pos[1] = CreateWidget<CEditorInput>("PosY", 80.f, 30.f);
	m_Pos[1]->SetHideName("PosY");
	m_Pos[1]->SetInputType(EImGuiInputType::Float);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_Pos[2] = CreateWidget<CEditorInput>("PosZ", 80.f, 30.f);
	m_Pos[2]->SetHideName("PosZ");
	m_Pos[2]->SetInputType(EImGuiInputType::Float);


	// ============== Rotation
	Label = CreateWidget<CEditorLabel>("회전");

	Label->SetColor(29, 47, 73, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSizeY(30.f);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_Rot[0] = CreateWidget<CEditorInput>("RotX", 80.f, 30.f);
	m_Rot[0]->SetHideName("RotX");
	m_Rot[0]->SetInputType(EImGuiInputType::Float);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_Rot[1] = CreateWidget<CEditorInput>("RotY", 80.f, 30.f);
	m_Rot[1]->SetHideName("RotY");
	m_Rot[1]->SetInputType(EImGuiInputType::Float);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_Rot[2] = CreateWidget<CEditorInput>("RotZ", 80.f, 30.f);
	m_Rot[2]->SetHideName("RotZ");
	m_Rot[2]->SetInputType(EImGuiInputType::Float);


	// ============== Scale
	Label = CreateWidget<CEditorLabel>("크기");

	Label->SetColor(29, 47, 73, 255);
	Label->SetAlign(0.5f, 0.5f);
	Label->SetSizeY(30.f);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_Scale[0] = CreateWidget<CEditorInput>("ScaleX", 80.f, 30.f);
	m_Scale[0]->SetHideName("ScaleX");
	m_Scale[0]->SetInputType(EImGuiInputType::Float);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_Scale[1] = CreateWidget<CEditorInput>("ScaleY", 80.f, 30.f);
	m_Scale[1]->SetHideName("ScaleY");
	m_Scale[1]->SetInputType(EImGuiInputType::Float);

	Line = CreateWidget<CEditorSameLine>("Line");

	m_Scale[2] = CreateWidget<CEditorInput>("ScaleZ", 80.f, 30.f);
	m_Scale[2]->SetHideName("ScaleZ");
	m_Scale[2]->SetInputType(EImGuiInputType::Float);

	m_Pos[0]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::PosXCallback);
	m_Pos[1]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::PosYCallback);
	m_Pos[2]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::PosZCallback);

	m_Rot[0]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::RotXCallback);
	m_Rot[1]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::RotYCallback);
	m_Rot[2]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::RotZCallback);

	m_Scale[0]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::ScaleXCallback);
	m_Scale[1]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::ScaleYCallback);
	m_Scale[2]->SetInputCallback<CTransformWindow>(this, &CTransformWindow::ScaleZCallback);

	return true;
}

void CTransformWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);

	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
			m_SelectComponent = nullptr;
	}
}

void CTransformWindow::PosXCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativePositionX(m_Pos[0]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldPositionX(m_Pos[0]->GetFloat());
		}
	}
}

void CTransformWindow::PosYCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativePositionY(m_Pos[1]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldPositionY(m_Pos[1]->GetFloat());
		}
	}
}

void CTransformWindow::PosZCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativePositionZ(m_Pos[2]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldPositionZ(m_Pos[2]->GetFloat());
		}
	}
}

void CTransformWindow::RotXCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeRotationX(m_Rot[0]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldRotationX(m_Rot[0]->GetFloat());
		}
	}
}

void CTransformWindow::RotYCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeRotationY(m_Rot[1]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldRotationY(m_Rot[1]->GetFloat());
		}
	}
}

void CTransformWindow::RotZCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeRotationZ(m_Rot[2]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldRotationZ(m_Rot[2]->GetFloat());
		}
	}
}

void CTransformWindow::ScaleXCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeScaleX(m_Scale[0]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldScaleX(m_Scale[0]->GetFloat());
		}
	}
}

void CTransformWindow::ScaleYCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeScaleY(m_Scale[1]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldScaleY(m_Scale[1]->GetFloat());
		}
	}
}

void CTransformWindow::ScaleZCallback()
{
	if (m_SelectComponent)
	{
		if (m_SelectComponent->GetParent())
		{
			m_SelectComponent->SetRelativeScaleZ(m_Scale[2]->GetFloat());
		}

		else
		{
			m_SelectComponent->SetWorldScaleZ(m_Scale[2]->GetFloat());
		}
	}
}
