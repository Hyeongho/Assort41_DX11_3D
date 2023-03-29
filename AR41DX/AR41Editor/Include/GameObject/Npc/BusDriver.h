#pragma once

#include "Npc.h"

enum class EBusState {
	MoveToBusStop,
	Stop,
	MoveFromBusStop,
	End
};

class CBusDriver
	: public CNpc
{
	friend class CScene;

protected:
	CBusDriver();
	CBusDriver(const CBusDriver& Obj);
	virtual ~CBusDriver();

private :
	Vector3		m_BusStopPos;
	bool		m_BusStart;
	EBusState	m_BusState;
	EMapList	m_PurposeScene;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CBusDriver* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void SetPurposeScene(const EMapList& Map)
	{
		m_PurposeScene = Map;
	}

public: // Bus Control
	void MoveToBusStop(const Vector3& BusStopPos, const Vector3& BusStartPos); // 버스 시작점으로부터 정류장으로 이동
	void MoveFromBusStop(); // 버스 정류장으로부터 출발하여 이동

private: // Animation
	void CreateAnim();
	void ChangeAnim_Stop();
	void ChangeAnim_Drive();
	
private:
	void BusMoveCutScene();	// 버스 이동용 컷씬 실행 함수
	void ChangeScene();		// 씬 변경용 함수, m_PurposeScene에 지정된 씬으로 이동한다.

private: // Debug key
	void DebugKeyF1();
	void DebugKeyF2();
	void DebugKeyF3();
};

