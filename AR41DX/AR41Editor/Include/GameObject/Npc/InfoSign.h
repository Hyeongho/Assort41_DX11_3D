#pragma once

#include "Npc.h"

class CInfoSign
	: public CNpc
{
	friend class CScene;

protected:
	CInfoSign();
	CInfoSign(const CInfoSign& Obj);
	virtual ~CInfoSign();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CInfoSign* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	virtual void StartDialog();

private:
	void DebugKeyF1();
	void DebugKeyF2();

};

