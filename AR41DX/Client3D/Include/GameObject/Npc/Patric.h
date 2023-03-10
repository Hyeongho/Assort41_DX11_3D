#pragma once

#include "Npc.h"

class CPatric
	: public CNpc
{
	friend class CScene;

protected:
	CPatric();
	CPatric(const CPatric& Obj);
	virtual ~CPatric();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPatric* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	virtual void ChangeAnimByName(const std::string& Name);
	virtual void StartDialog();

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

