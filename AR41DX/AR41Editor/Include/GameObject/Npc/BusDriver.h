#pragma once

#include "Npc.h"

class CBusDriver
	: public CNpc
{
	friend class CScene;

protected:
	CBusDriver();
	CBusDriver(const CBusDriver& Obj);
	virtual ~CBusDriver();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CBusDriver* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	virtual void StartDialog();

public:
	void MoveToBusStop();

private:
	void ChangeAnim_Stop();
	void ChangeAnim_Drive();

private:
	void DebugKeyF1();
	void DebugKeyF2();

};

