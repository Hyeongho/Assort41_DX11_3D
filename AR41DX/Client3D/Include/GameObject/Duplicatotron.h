#pragma once

#include "Monster.h"

class CDuplicatotron :
	public CMonster
{
	friend class CScene;

protected:
	CDuplicatotron();
	CDuplicatotron(const CDuplicatotron& Obj);
	virtual ~CDuplicatotron();

private:
//	CSharedPtr<class CHammer>	m_Hammer;
	bool		m_SpawnOn;
	float		m_DelayTime;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CDuplicatotron* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	void SpawnAnimation();
	void SpawnHammers();
};
