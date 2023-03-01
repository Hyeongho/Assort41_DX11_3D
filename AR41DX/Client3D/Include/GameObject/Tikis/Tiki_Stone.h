#pragma once

#include "GameObject\GameObject.h"

class CTiki_Stone :
	public CGameObject
{
	friend class CScene;

protected:
	CTiki_Stone();
	CTiki_Stone(const CTiki_Stone& Obj);
	virtual ~CTiki_Stone();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CRigidBody>				m_Rigid;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTiki_Stone* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void ChangeAnim_Idle();
	void ChangeAnim_Die();

private:
	void Tiki_Die(); // ÆÄ±«Ã³¸®

};