#include "ColliderObb3dWindow.h"
#include "CollisionManager.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorCheckBox.h"
#include "Component/ColliderObb3d.h"

CColliderObb3dWindow::CColliderObb3dWindow()
	: m_ChannelList(nullptr)
	, m_ProfileRelationCheck(nullptr)
	, m_ColiderSizeText{}
	, m_ProfileName(nullptr)
{
}

CColliderObb3dWindow::~CColliderObb3dWindow()
{
}

void CColliderObb3dWindow::SetSelectComponent(CCollider* component)
{
	m_SelectObb = (CColliderOBB3D*)component;
	m_ColiderSizeText[0]->SetFloat(m_SelectObb->GetInfo().Length[0]);
	m_ColiderSizeText[1]->SetFloat(m_SelectObb->GetInfo().Length[1]);
	m_ColiderSizeText[2]->SetFloat(m_SelectObb->GetInfo().Length[2]);
	m_ProfileName->SetText(m_SelectObb->GetCollisionProfile()->Name.c_str());
}

bool CColliderObb3dWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("콜라이더 OBB");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_ProfileRelationCheck = CreateWidget<CEditorCheckBox>("충돌");
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("설정", 50.f, 30.f);
	button->SetClickCallback<CColliderObb3dWindow>(this, &CColliderObb3dWindow::ChannelChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("적용", 50.f, 30.f);
	button->SetClickCallback<CColliderObb3dWindow>(this, &CColliderObb3dWindow::RelationChangeCallback);

	m_ProfileName = CreateWidget<CEditorInput>("profile", 120.f, 30.f);
	m_ProfileName->SetHideName("profile");
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("프로파일 생성", 100.f, 30.f);
	button->SetClickCallback<CColliderObb3dWindow>(this, &CColliderObb3dWindow::ProfileCreateCallback);
	m_ChannelList = CreateWidget<CEditorListBox>("ChannelListBox");
	m_ChannelList->SetHideName("ChannelListBox");
	m_ChannelList->SetSize(200.f, 300.f);
	m_ChannelList->SetPageItemCount(6);
	m_ChannelList->SetSelectCallback<CColliderObb3dWindow>(this, &CColliderObb3dWindow::ChannelClickCallback);
	label = CreateWidget<CEditorLabel>("사이즈");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_ColiderSizeText[0] = CreateWidget<CEditorInput>("SizeX", 80.f, 30.f);
	m_ColiderSizeText[0]->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText[0]->SetHideName("SizeX");
	m_ColiderSizeText[0]->SetInputCallback<CColliderObb3dWindow>(this, &CColliderObb3dWindow::ColiderChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ColiderSizeText[1] = CreateWidget<CEditorInput>("SizeY", 80.f, 30.f);
	m_ColiderSizeText[1]->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText[1]->SetHideName("SizeY");
	m_ColiderSizeText[1]->SetInputCallback<CColliderObb3dWindow>(this, &CColliderObb3dWindow::ColiderChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ColiderSizeText[2] = CreateWidget<CEditorInput>("SizeZ", 80.f, 30.f);
	m_ColiderSizeText[2]->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText[2]->SetHideName("SizeZ");
	m_ColiderSizeText[2]->SetInputCallback<CColliderObb3dWindow>(this, &CColliderObb3dWindow::ColiderChangeCallback);
	LoadChannelName();
	return true;
}

void CColliderObb3dWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectObb)
	{
		if (!m_SelectObb->GetActive())
		{
			m_SelectObb = nullptr;
		}
	}
}

void CColliderObb3dWindow::ChannelClickCallback(int index, const std::string& item)
{
	m_SelectRelationItem = item;
	bool isCollision = CCollisionManager::GetInst()->CheckCollisionInteraction(m_ProfileName->GetText()
		, m_SelectRelationItem, ECollision_Interaction::Collision);
	m_ProfileRelationCheck->SetCheck(isCollision);
}

void CColliderObb3dWindow::ProfileCreateCallback()
{
	std::string name(m_ProfileName->GetText());
	if (name == "")
	{
		return;
	}
	CCollisionManager::GetInst()->CreateChannel(name, ECollision_Interaction::Collision);
	CCollisionManager::GetInst()->CreateProfile(name, name, true);
	m_ChannelList->Clear();
	LoadChannelName();
}

void CColliderObb3dWindow::ColiderChangeCallback()
{
	if (!m_SelectObb)
	{
		return;
	}
	float sizeX = m_ColiderSizeText[0]->GetFloat();
	float sizeY = m_ColiderSizeText[1]->GetFloat();
	float sizeZ = m_ColiderSizeText[2]->GetFloat();
	m_SelectObb->SetBoxHalfSize(sizeX, sizeY, sizeZ);
}

void CColliderObb3dWindow::ChannelChangeCallback()
{
	if (!m_SelectObb)
	{
		return;
	}
	m_SelectObb->SetCollisionProfile(m_SelectRelationItem);
}

void CColliderObb3dWindow::RelationChangeCallback()
{
	ECollision_Interaction interaction =
		m_ProfileRelationCheck->GetCheck() ? ECollision_Interaction::Collision : ECollision_Interaction::Ignore;
	CCollisionManager::GetInst()->SetCollisionInteraction(m_ProfileName->GetText(), m_SelectRelationItem, interaction);
}

void CColliderObb3dWindow::LoadChannelName()
{
	std::vector<std::string> vecName;
	CCollisionManager::GetInst()->GetProfileNames(vecName);
	size_t size = vecName.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_ChannelList->AddItem(vecName[i]);
	}
}