#include "ColliderCubeWindow.h"
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
#include "Component/ColliderCube.h"

CColliderCubeWindow::CColliderCubeWindow()
	: m_ChannelList(nullptr)
	, m_ProfileRelationCheck(nullptr)
	, m_ColiderSizeText{}
	, m_ProfileName(nullptr)
{
}

CColliderCubeWindow::~CColliderCubeWindow()
{
}

void CColliderCubeWindow::SetSelectComponent(CCollider* component)
{
	m_SelectCube = (CColliderCube*)component;
	m_ColiderSizeText[0]->SetFloat(m_SelectCube->GetCubeSize().x);
	m_ColiderSizeText[1]->SetFloat(m_SelectCube->GetCubeSize().y);
	m_ColiderSizeText[2]->SetFloat(m_SelectCube->GetCubeSize().z);
	m_ProfileName->SetText(m_SelectCube->GetCollisionProfile()->Name.c_str());
}

bool CColliderCubeWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("콜라이더 큐브");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_ProfileRelationCheck = CreateWidget<CEditorCheckBox>("충돌");
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("설정", 50.f, 30.f);
	button->SetClickCallback<CColliderCubeWindow>(this, &CColliderCubeWindow::ChannelChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("적용", 50.f, 30.f);
	button->SetClickCallback<CColliderCubeWindow>(this, &CColliderCubeWindow::RelationChangeCallback);

	m_ProfileName = CreateWidget<CEditorInput>("profile", 120.f, 30.f);
	m_ProfileName->SetHideName("profile");
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("프로파일 생성", 100.f, 30.f);
	button->SetClickCallback<CColliderCubeWindow>(this, &CColliderCubeWindow::ProfileCreateCallback);
	m_ChannelList = CreateWidget<CEditorListBox>("ChannelListBox");
	m_ChannelList->SetHideName("ChannelListBox");
	m_ChannelList->SetSize(200.f, 300.f);
	m_ChannelList->SetPageItemCount(6);
	m_ChannelList->SetSelectCallback<CColliderCubeWindow>(this, &CColliderCubeWindow::ChannelClickCallback);
	label = CreateWidget<CEditorLabel>("사이즈");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_ColiderSizeText[0] = CreateWidget<CEditorInput>("SizeX", 80.f, 30.f);
	m_ColiderSizeText[0]->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText[0]->SetHideName("SizeX");
	m_ColiderSizeText[0]->SetInputCallback<CColliderCubeWindow>(this, &CColliderCubeWindow::ColiderChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ColiderSizeText[1] = CreateWidget<CEditorInput>("SizeY", 80.f, 30.f);
	m_ColiderSizeText[1]->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText[1]->SetHideName("SizeY");
	m_ColiderSizeText[1]->SetInputCallback<CColliderCubeWindow>(this, &CColliderCubeWindow::ColiderChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ColiderSizeText[2] = CreateWidget<CEditorInput>("SizeZ", 80.f, 30.f);
	m_ColiderSizeText[2]->SetInputType(EImGuiInputType::Float);
	m_ColiderSizeText[2]->SetHideName("SizeZ");
	m_ColiderSizeText[2]->SetInputCallback<CColliderCubeWindow>(this, &CColliderCubeWindow::ColiderChangeCallback);
	LoadChannelName();
	return true;
}

void CColliderCubeWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectCube)
	{
		if (!m_SelectCube->GetActive())
		{
			m_SelectCube = nullptr;
		}
	}
}

void CColliderCubeWindow::ChannelClickCallback(int index, const std::string& item)
{
	m_SelectRelationItem = item;
	bool isCollision = CCollisionManager::GetInst()->CheckCollisionInteraction(m_ProfileName->GetText()
		, m_SelectRelationItem, ECollision_Interaction::Collision);
	m_ProfileRelationCheck->SetCheck(isCollision);
}

void CColliderCubeWindow::ProfileCreateCallback()
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

void CColliderCubeWindow::ColiderChangeCallback()
{
	if(!m_SelectCube)
	{
		return;
	}
	float sizeX = m_ColiderSizeText[0]->GetFloat();
	float sizeY = m_ColiderSizeText[1]->GetFloat();
	float sizeZ = m_ColiderSizeText[2]->GetFloat();
	m_SelectCube->SetCubeSize(sizeX, sizeY, sizeZ);
}

void CColliderCubeWindow::ChannelChangeCallback()
{
	if (!m_SelectCube)
	{
		return;
	}
	m_SelectCube->SetCollisionProfile(m_SelectRelationItem);
}

void CColliderCubeWindow::RelationChangeCallback()
{
	ECollision_Interaction interaction =
		m_ProfileRelationCheck->GetCheck() ? ECollision_Interaction::Collision : ECollision_Interaction::Ignore;
	CCollisionManager::GetInst()->SetCollisionInteraction(m_ProfileName->GetText(), m_SelectRelationItem, interaction);
}

void CColliderCubeWindow::LoadChannelName()
{
	std::vector<std::string> vecName;
	CCollisionManager::GetInst()->GetProfileNames(vecName);
	size_t size = vecName.size();
	for (size_t i = 0; i < size; ++i)
	{
		m_ChannelList->AddItem(vecName[i]);
	}
}