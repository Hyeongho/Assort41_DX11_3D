#pragma once

#include "GameObject\GameObject.h"

class CPatric
	: public CGameObject
{
	friend class CScene;

protected:
	CPatric();
	CPatric(const CPatric& Obj);
	virtual ~CPatric();

protected:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPatric* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void ChangeAnim_Confused();
	void ChangeAnim_Default();
	void ChangeAnim_Excited();
	void ChangeAnim_Scowl_Start();
	void ChangeAnim_Scowl_Loop();
	void ChangeAnim_Talk();
	void ChangeAnim_Thinking_Start();
	void ChangeAnim_Thinking_Loop();
};

