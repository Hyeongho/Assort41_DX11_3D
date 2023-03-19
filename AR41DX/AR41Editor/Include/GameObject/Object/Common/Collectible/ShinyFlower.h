#pragma once

#include "CollectibleItems.h"

enum class EFlowerColor {
	Red,
	Yellow,
	Green,
	Blue,
	Purple,
	End
};

class CShinyFlower :
	public CCollectibleItems
{
	friend class CScene;

protected:
	CShinyFlower();
	CShinyFlower(const CShinyFlower& Obj);
	virtual ~CShinyFlower();

private :
	bool			m_FixedFlower; // ������ ������Ʈ���� üũ. �������� ���, �÷��̾ üũ�Ͽ� ������� �ʴ´�.
	bool			m_FoundPlayer; // ������ ������Ʈ�� �ƴ� ���, PlayerChecked �Լ��� �ش� ������ true�� �� ��� �÷��̾ �����Ѵ�.
	EFlowerColor	m_FlowerColor;

	CSharedPtr<class CColliderOBB3D>			m_ColliderPlCheckRange; // ������ ������Ʈ�� �ƴ� ���, �÷��̾ �����ϱ� ���� �浹ü.

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CShinyFlower* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	virtual void Collision_Player(const CollisionResult& result);  // �����ۿ� ����� �� ���� ó��
	void Collision_Range(const CollisionResult& result); // �÷��̾ ���� �ȿ� ���� �� ������ �����ϱ� ���� �浹 ó��

private:
	void SetRandColor();

public :
	void SetFixedFlower() {
		m_FixedFlower = true;
	}
};

