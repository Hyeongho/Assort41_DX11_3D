#pragma once

#include "GameObject/GameObject.h"

enum class EAttackWordType {
	KAH,
	RAH,
	TAE
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
	EAttackWordType		m_AttackWordType;	// ���� ���� Ÿ��
	bool				m_MoveToPlatform;	// �̵� ���� ó��
	Vector3				m_DestPos;			// ��ǥ �̵� ����

	bool				m_StartTimer;		// ��ǥ ���� ���� ��, ���ϱ����� ���ð� ī���� Ʈ����
	float				m_AttackDelayTimer; // ��ǥ ���� ���� ��, ���ϱ����� ���ð� ī����(2s)
	bool				m_Drop;				// ���ð� ī���� ��, ���� Ʈ����

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CRoboSponge_AttackWords* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	void Collision(const CollisionResult& result);

public:
	void SetWordType(EAttackWordType WordType);		// ���� Ÿ�� ����
	void SetDestPos(const Vector3& DestPos) { m_DestPos = DestPos; };	// �̵� ��ǥ ���� ����.
	void FallWords(bool isFall = true) { m_MoveToPlatform = isFall; }

};

