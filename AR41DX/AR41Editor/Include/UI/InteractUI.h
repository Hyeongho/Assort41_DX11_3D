﻿#pragma once

#include "UI/UIWindow.h"

enum class EInteractTarget {
	Npc,			// NPC
	Sponge_House,	// 스폰지밥 집
	Squid_House,	// 징징이 집
	Patric_House,	// 뚱이 집
	Door,			// 기타 문
	Gate,			// 차단기
	Teleport_Box,	// 순간이동 박스
	Bus,			// 버스 정류소
	Taxi,			// 택시
	InfoSign,		// 인포사인
	InfoSign2,		// 인포사인
	End
};

class CInteractUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CInteractUI();
	CInteractUI(const CInteractUI& Window);
	virtual ~CInteractUI();

protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapInteractUI;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CInteractUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void CreaeteAllUI();
	void CreateInteractUI();

public :
	void ActiveInteractUI();
	void InActiveInteractUI();

public :
	void SetTarget(EInteractTarget Target);
};