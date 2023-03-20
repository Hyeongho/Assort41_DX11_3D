#include "ShinyFlower.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "../../../Player.h"

CShinyFlower::CShinyFlower() :
	m_FixedFlower(false),
	m_FoundPlayer(false)
{
	SetTypeID<CShinyFlower>();

	m_ObjectTypeName = "ShinyFlower";

	m_FlowerColor = EFlowerColor::Yellow;
	m_ColItemType = EColItemType::ShinyFlower;
}

CShinyFlower::CShinyFlower(const CShinyFlower& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

	m_FixedFlower = Obj.m_FixedFlower;

	if (!m_FixedFlower) {
		m_ColliderPlCheckRange = (CColliderOBB3D*)FindComponent("OBB3D");
		m_FoundPlayer = false;
	}

	m_ColItemType = Obj.m_ColItemType;
}

CShinyFlower::~CShinyFlower()
{
}

void CShinyFlower::Start()
{
	CCollectibleItems::Start();

	if (!m_FixedFlower) {
		int RandSound = rand() % 7 + 1; // 1 ~ 7
		std::string SoundName = "CreateShiny" + std::to_string(RandSound);

		CSound* Sound = m_Scene->GetResource()->FindSound(SoundName);

		if (Sound)
			Sound->Play();
	}
}

bool CShinyFlower::Init()
{
	CCollectibleItems::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");
	m_ColliderPlCheckRange = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Collider);
	m_Mesh->AddChild(m_ColliderPlCheckRange);
	m_Mesh->SetMesh("YellowFlower");

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CShinyFlower>(ECollision_Result::Collision, this, &CShinyFlower::Collision_Player);

	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);


	// �÷��̾� Ž���� �浹ü
	Vector3 ColSize(150.f, 150.f, 150.f);

	m_ColliderPlCheckRange->SetBoxHalfSize(ColSize);
	m_ColliderPlCheckRange->SetRelativePositionY(ColSize.y / 2.f);
	m_ColliderPlCheckRange->SetCollisionProfile("Collectible");
	m_ColliderPlCheckRange->SetCollisionCallback<CShinyFlower>(ECollision_Result::Collision, this, &CShinyFlower::Collision_Range);

	m_ColliderPlCheckRange->SetInheritRotX(true);
	m_ColliderPlCheckRange->SetInheritRotY(true);
	m_ColliderPlCheckRange->SetInheritRotZ(true);


	SetRandColor();


	return true;
}

void CShinyFlower::Update(float DeltaTime)
{
	CCollectibleItems::Update(DeltaTime);

	// �÷��̾ ã���� ��.
	if (m_FoundPlayer && !m_FixedFlower) {
		CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

		if (!Player)
			return;

		Vector3 PlayerPos = Player->GetWorldPos();

		Vector3 Dir = PlayerPos - GetWorldPos();
		
		Dir.Normalize();

		// m_MoveSpeed�� Monster Ŭ�������� 100.f�� �Ǿ��ִ�. �ӵ� 1.5�� ������.
		m_Mesh->AddWorldPosition(Dir * 500.f * g_DeltaTime);
	}
}

void CShinyFlower::PostUpdate(float DeltaTime)
{
	CCollectibleItems::PostUpdate(DeltaTime);
}

CShinyFlower* CShinyFlower::Clone() const
{
	return new CShinyFlower(*this);
}

void CShinyFlower::Save(FILE* File)
{
	CCollectibleItems::Save(File);
}

void CShinyFlower::Load(FILE* File)
{
	CCollectibleItems::Load(File);
}

void CShinyFlower::Collision_Player(const CollisionResult& result)
{
	CCollectibleItems::Collision_Player(result);

	int FlowerCount = 20 * ((int)m_FlowerColor + 1);

	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (Player) {
		// �÷��̾� �ö�� ���� ����
		// Player->AddFlowerCount(FlowerCount);
	}
	
	// ���� ó��
	CSound* Sound = m_Scene->GetResource()->FindSound("GetShiny");

	if (Sound)
		Sound->Play();

	// ������Ʈ ����ó��
	Destroy();
}

void CShinyFlower::Collision_Range(const CollisionResult& result)
{
	m_FoundPlayer = true;
}

void CShinyFlower::SetRandColor()
{
	int ColorNum = rand() % 5;

	m_FlowerColor = (EFlowerColor)ColorNum;
	m_Mesh->ClearMaterial();
	
	switch (m_FlowerColor)
	{
	case EFlowerColor::Red:
		m_Mesh->SetMesh("RedFlower");
		break;
	case EFlowerColor::Yellow:
		m_Mesh->SetMesh("YellowFlower");
		break;
	case EFlowerColor::Green:
		m_Mesh->SetMesh("GreenFlower");
		break;
	case EFlowerColor::Blue:
		m_Mesh->SetMesh("BlueFlower");
		break;
	case EFlowerColor::Purple:
		m_Mesh->SetMesh("PurpleFlower");
		break;
	}
}
