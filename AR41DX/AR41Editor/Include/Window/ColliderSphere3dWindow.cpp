#include "ColliderSphere3dWindow.h"
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
#include "Component/ColliderSphere3d.h"

CColliderSphere3dWindow::CColliderSphere3dWindow()
	: m_ChannelList(nullptr)
	, m_ProfileRelationCheck(nullptr)
	, m_ColiderSizeText{}
	, m_ProfileName(nullptr)
{
}

CColliderSphere3dWindow::~CColliderSphere3dWindow()
{
}

void CColliderSphere3dWindow::SetSelectComponent(CCollider* component)
{
	m_SelectSphere = (CColliderSphere3D*)component;
	m_ColiderSizeText->SetFloat(m_SelectSphere->GetInfo().Radius);
	m_ProfileName->SetText(m_SelectSphere->GetCollisionProfile()->Name.c_str());
}

bool CColliderSphere3dWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("콜라이더 구");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_ProfileRelationCheck = CreateWidget<CEditorCheckBox>("충돌");
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("설정", 50.f, 30.f);
	button->SetClickCallback<CColliderSphere3dWindow>(this, &CColliderSphere3dWindow::ChannelChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("적용", 50.f, 30.f);
	button->SetClickCallback<CColliderSphere3dWindow>(this, &CColliderSphere3dWindow::RelationChangeCallback);

	m_ProfileName = CreateWidget<CEditorInput>("profile", 120.f, 30.f);
	m_ProfileName->SetHideName("profile");
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("프로파일 생성", 100.f, 30.f);
	button->SetClickCallback<CColliderSphere3dWindow>(this, &CColliderSphere3dWindow::ProfileCreateCallback);
	m_ChannelList = CreateWidget<CEditorListBox>("ChannelListBox");
	m_ChannelList->SetHideName("ChannelListBox");
	m_ChannelList->SetSize(200.f, 300.f);
	m_ChannelList->SetPageItemCount(6);
	m_ChannelList->SetSelectCallback<CColliderSphere3dWindow>(this, &CColliderSphere3dWindow::ChannelClickCallback);
	label = CreateWidget<CEditorLabel>("사이즈");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_ColiderSizeText = CreateWidget<CEditorInput>("Size", 80.f, 30.f);
	m_ColiderSizeText->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText->SetHideName("Size");
	m_ColiderSizeText->SetInputCallback<CColliderSphere3dWindow>(this, &CColliderSphere3dWindow::ColiderChangeCallback);
	LoadChannelName();
	return true;
}

void CColliderSphere3dWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectSphere)
	{
		if (!m_SelectSphere->GetActive())
		{
			m_SelectSphere = nullptr;
		}
	}
}

void CColliderSphere3dWindow::ChannelClickCallback(int index, const std::string& item)
{
	m_SelectRelationItem = item;
	bool isCollision = CCollisionManager::GetInst()->CheckCollisionInteraction(m_ProfileName->GetText()
		, m_SelectRelationItem, ECollision_Interaction::Collision);
	m_ProfileRelationCheck->SetCheck(isCollision);
}

void CColliderSphere3dWindow::ProfileCreateCallback()
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

void CColliderSphere3dWindow::ColiderChangeCallback()
{
	if (!m_SelectSphere)
	{
		return;
	}
	float size = m_ColiderSizeText->GetFloat();
	m_SelectSphere->SetRadius(size);
}

void CColliderSphere3dWindow::ChannelChangeCallback()
{
	if (!m_SelectSphere)
	{
		return;
	}
	m_SelectSphere->SetCollisionProfile(m_SelectRelationItem);
}

void CColliderSphere3dWindow::RelationChangeCallback()
{
	ECollision_Interaction interaction =
		m_ProfileRelationCheck->GetCheck() ? ECollision_Interaction::Collision : ECollision_Interaction::Ignore;
	CCollisionManager::GetInst()->SetCollisionInteraction(m_ProfileName->GetText(), m_SelectRelationItem, interaction);
}

void CColliderSphere3dWindow::LoadChannelName()
{
	std::vector<std::string> vecName;
	CCollisionManager::GetInst()->GetProfileNames(vecName);
	size_t size = vecName.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_ChannelList->AddItem(vecName[i]);
	}
}