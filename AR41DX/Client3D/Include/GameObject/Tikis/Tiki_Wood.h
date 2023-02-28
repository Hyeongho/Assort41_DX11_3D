#pragma once

#include "GameObject\GameObject.h"

class CTiki_Wood :
	public CGameObject
{
	friend class CScene;

protected:
	CTiki_Wood();
	CTiki_Wood(const CTiki_Wood& Obj);
	virtual ~CTiki_Wood();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CRigidBody>				m_Rigid;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTiki_Wood* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void ChangeAnim_Idle();
	void ChangeAnim_Die();

private :
	void Tiki_Die(); // ÆÄ±«Ã³¸®

};

