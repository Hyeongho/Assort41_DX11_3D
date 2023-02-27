#pragma once

#include "EngineInfo.h"
#include "EngineSetting.h"

class CEngine
{
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	Resolution m_WindowRS;
	float m_ClearColor[4];
	static bool	m_Loop;
	class CTimer* m_Timer;
	static bool m_EditorMode;
	static std::function<bool(HWND, UINT, WPARAM, LPARAM)> m_WndProcFunc;
	CEngineSetting* m_Setting;
	bool m_Render2D;
	class CGlobalConstantBuffer* m_GlobalCBuffer;
	float m_AccTime;
	float m_TimeScale; // 글로벌 타임 재생 배율(게임 전체의 시간을 조절할때 씀)
	class CStructuredBuffer* m_RandomBuffer;

public:
	class CGlobalConstantBuffer* GetGlobalCBuffer()	const
	{
		return m_GlobalCBuffer;
	}

	bool GetRender2D()	const
	{
		return m_Render2D;
	}

	void EnableRender2D()
	{
		m_Render2D = true;
	}

	float GetFPS()	const;
	float GetDeltaTime()	const;

	static bool GetEditorMode()
	{
		return m_EditorMode;
	}

	static void EnableEditor()
	{
		m_EditorMode = true;
	}

	Resolution GetWindowResolution()	const
	{
		return m_WindowRS;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

	void SetCameraAxisX(const Vector3& Axis);
	void SetCameraAxisY(const Vector3& Axis);

	//김범중 게임 일시정지 변수 관리 함수
	float GetTimeScale()	const
	{
		return m_TimeScale;
	}

	void SetTimeScale(float scale)
	{
		m_TimeScale = scale;
	}

public:
	bool Init(HINSTANCE hInst, const TCHAR* Title,
		const TCHAR* ClassName, int IconID, int SmallIconID,
		unsigned int WindowWidth,
		unsigned int WindowHeight, 
		unsigned int DeviceWidth, unsigned int DeviceHeight,
		bool WindowMode = true, int MenuID = 0);
	int Run();

private:
	void Logic();
	void Input(float DeltaTime);
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);
	void Render(float DeltaTime);


private:
	void Register(const TCHAR* ClassName, int IconID, int SmallIconID, int MenuID);
	bool Create(const TCHAR* Title, const TCHAR* ClassName);

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	template <typename T>
	static void SetWndProcCallback(T* Obj, bool(T::* Func)(HWND, UINT, WPARAM, LPARAM))
	{
		m_WndProcFunc = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4);
	}
	
	template <typename T>
	void SetEngineSetting()
	{
		SAFE_DELETE(m_Setting);

		m_Setting = new T;

		m_Setting->Init();
	}

	DECLARE_SINGLE(CEngine)
};

