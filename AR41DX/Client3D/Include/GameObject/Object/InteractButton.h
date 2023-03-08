#pragma once

#include "GameObject\GameObject.h"

class CInteractButton
	: public CGameObject
{
	friend class CScene;

protected:
	CInteractButton();
	CInteractButton(const CInteractButton& Obj);
	virtual ~CInteractButton();

protected:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;
	CSharedPtr<class CColliderCube>				m_Cube;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CInteractButton* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

