#include "Timer.h"
#include "Engine.h"

CTimer::CTimer() :
	m_Second{},
	m_Time{},
	m_DeltaTime(0.f),
	m_FPS(0.f),
	m_FPSTime(0.f),
	m_Tick(0)
{
}

CTimer::~CTimer()
{
}

void CTimer::Init()
{
	// 고해상도 타이머가 1초가 흘렀을때의 값을 얻어온다.
	QueryPerformanceFrequency(&m_Second);

	// 현재 고해상도 타이머의 값을 얻어온다.
	QueryPerformanceCounter(&m_Time);
}

void CTimer::Update()
{
	LARGE_INTEGER	Time;
	QueryPerformanceCounter(&Time);

	// DeltaTime을 구한다.
	// (현재 고해상도 타이머값 - 이전 프레임의 고해상도 타이머값) / 초당 고해상도 타이머값
	m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

	// 이전 타이머값을 현재 타이머값으로 교체한다.
	m_Time = Time;

	m_FPSTime += m_DeltaTime;
	++m_Tick;

	if (m_Tick == 60)
	{
		m_FPS = m_Tick / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;
	}
}

