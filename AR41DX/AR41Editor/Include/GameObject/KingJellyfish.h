#pragma once
#include "GameObject\GameObject.h"

class CKingJellyfish :
	public CGameObject
{
	friend class CScene;

protected:
	CKingJellyfish();
	CKingJellyfish(const CKingJellyfish& Obj);
	virtual ~CKingJellyfish();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>		m_Animation;

private:
	float	m_AttackTime; // ���� ��� �� ��Ÿ�� 

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CKingJellyfish* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void Damage(); // �÷��̾�� ���� ���ϴ� ����
	void Angry(); // �÷��̾�� ���� ���� ����
	void OnGround(); // ���� ����1, ������ �����ϸ� �� ����� ����ĸ� ����
	void Attack(); // ���� ����2, ���� ������ ���·� �� ����� ����ĸ� ����
	void SpawnJellyfish(); // ���� ����2, ���ĸ� ���͸� ����
};

