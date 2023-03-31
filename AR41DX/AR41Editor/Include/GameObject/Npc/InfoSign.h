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

private :
	bool		m_RotLeft;
	bool		m_RotRight;
	std::string	m_InfoSignName;

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
	void InputF();


private:
	void DebugKeyF1();
	void DebugKeyF2();

public:
	void SetInfoSingName(const std::string& Name) {
		m_InfoSignName = Name;
	}
};