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
	bool			m_FixedFlower; // 고정형 오브젝트인지 체크. 고정형인 경우, 플레이어를 체크하여 따라오지 않는다.
	bool			m_FoundPlayer; // 고정형 오브젝트가 아닐 경우, PlayerChecked 함수로 해당 변수를 true로 할 경우 플레이어를 추적한다.
	EFlowerColor	m_FlowerColor;

	CSharedPtr<class CColliderOBB3D>			m_ColliderPlCheckRange; // 고정형 오브젝트가 아닐 경우, 플레이어를 추적하기 위한 충돌체.

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CShinyFlower* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	virtual void Collision_Player(const CollisionResult& result);  // 아이템에 닿았을 때 습득 처리
	void Collision_Range(const CollisionResult& result); // 플레이어가 범위 안에 들어갔을 때 추적을 시작하기 위한 충돌 처리

private:
	void SetRandColor();

public :
	void SetFixedFlower() {
		m_FixedFlower = true;
	}
};

