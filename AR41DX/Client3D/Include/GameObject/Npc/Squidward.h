#pragma once

#include "Npc.h"

class CSquidward
	: public CNpc
{
	friend class CScene;

protected:
	CSquidward();
	CSquidward(const CSquidward& Obj);
	virtual ~CSquidward();

protected:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation>				m_Animation;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSquidward* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

protected:
	virtual void StartDialog();

private:
	void ChangeAnim_Angry_Loop();
	void ChangeAnim_Angry_Start();
	void ChangeAnim_Annoyed_Loop();
	void ChangeAnim_Annoyed_Start();
	void ChangeAnim_Happy_Loop();
	void ChangeAnim_Happy_Start();	
	void ChangeAnim_Hurt_Loop();
	void ChangeAnim_Hurt_Start();
	void ChangeAnim_Sarcastic_Loop();
	void ChangeAnim_Sarcastic_Start();
	void ChangeAnim_Talk();
	void ChangeAnim_Talk_Idle();
	void ChangeAnim_Idle();
};

