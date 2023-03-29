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

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSquidward* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void SetBandageMode();

public:
	virtual void StartDialog();

private: // Animation
	void CreateAnim();
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

	void ChangeAnim_Intro1();
	void ChangeAnim_Intro2();
	void ChangeAnim_Outro1();
};

