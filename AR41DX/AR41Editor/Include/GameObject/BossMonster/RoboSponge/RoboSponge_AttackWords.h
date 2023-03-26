#pragma once

#include "GameObject/GameObject.h"

enum class EAttackWordType {
	KAE,
	RAE,
	TAE,
	End
};

class CRoboSponge_AttackWords :
	public CGameObject
{
	friend class CScene;

protected:
	CRoboSponge_AttackWords();
	CRoboSponge_AttackWords(const CRoboSponge_AttackWords& Obj);
	virtual ~CRoboSponge_AttackWords();

protected:
	CSharedPtr<class CStaticMeshComponent>		m_Mesh;
	CSharedPtr<class CColliderOBB3D>			m_Collider;

private:
	EAttackWordType		m_AttackWordType;	// 공격 워드 타입
	bool				m_MoveToPlatform;	// 이동 시작 처리
	Vector3				m_DestPos;			// 목표 이동 지점

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CRoboSponge_AttackWords* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	void Collision_Player(const CollisionResult& result);

public:
	void SetWordType(EAttackWordType WordType);		// 워드 타입 설정
	void SetDestPos(Vector3 DestPos);				// 이동 목표 지점 설정.

};

