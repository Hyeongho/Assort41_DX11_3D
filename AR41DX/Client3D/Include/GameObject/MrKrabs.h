#pragma once

#include "GameObject\GameObject.h"

class CMrKrabs
	: public CGameObject
{
	friend class CScene;

protected:
	CMrKrabs();
	CMrKrabs(const CMrKrabs& Obj);
	virtual ~CMrKrabs();

protected:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CMrKrabs* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void ChangeAnim_Angry_Loop();
	void ChangeAnim_Angry_Start();
	void ChangeAnim_Deceptive_Loop();
	void ChangeAnim_Deceptive_Start();
	void ChangeAnim_Greedy_Loop();
	void ChangeAnim_Greedy_Start();
	void ChangeAnim_Laughing();
	void ChangeAnim_Idle();
};

