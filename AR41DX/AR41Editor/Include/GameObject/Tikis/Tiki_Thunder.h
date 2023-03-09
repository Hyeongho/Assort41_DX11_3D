#pragma once

#include "GameObject\GameObject.h"

class CTiki_Thunder :
	public CGameObject
{
	friend class CScene;

protected:
	CTiki_Thunder();
	CTiki_Thunder(const CTiki_Thunder& Obj);
	virtual ~CTiki_Thunder();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CRigidBody>				m_Rigid;
	CSharedPtr<class CColliderCube>				m_Cube;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTiki_Thunder* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void ChangeAnim_Idle();
	void ChangeAnim_Die();

private:
	void Tiki_Die(); // ÆÄ±«Ã³¸®
	void CreateFlowers();

private: // Collision
	void AttackedCollision(const CollisionResult& result);

};

