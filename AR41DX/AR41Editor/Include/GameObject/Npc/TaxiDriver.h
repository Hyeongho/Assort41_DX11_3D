#pragma once

#include "Npc.h"

class CTaxiDriver
	: public CNpc
{
	friend class CScene;

protected:
	CTaxiDriver();
	CTaxiDriver(const CTaxiDriver& Obj);
	virtual ~CTaxiDriver();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTaxiDriver* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void ChangeSceneToBB();
};
