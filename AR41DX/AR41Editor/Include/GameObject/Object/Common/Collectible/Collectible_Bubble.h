#pragma once

#include "GameObject\GameObject.h"

class CCollectible_Bubble
	: public CGameObject
{
	friend class CScene;

protected:
	CCollectible_Bubble();
	CCollectible_Bubble(const CCollectible_Bubble& Obj);
	virtual ~CCollectible_Bubble();

protected:
	CSharedPtr<class CStaticMeshComponent>		m_BubbleMesh;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCollectible_Bubble* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

