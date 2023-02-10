#pragma once

#include "GameObject\GameObject.h"

class CJellyfish :
	public CGameObject
{
	friend class CScene;

protected:
	CJellyfish();
	CJellyfish(const CJellyfish& Obj);
	virtual ~CJellyfish();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>		m_Animation;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CJellyfish* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void Attack();
	void Death();
};

