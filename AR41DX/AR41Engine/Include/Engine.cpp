#include "Engine.h"
#include "Device.h"
#include "Resource\ResourceManager.h"
#include "Timer.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "Input.h"
#include "Editor/EditorGUIManager.h"
#include "CollisionManager.h"
#include "Thread/ThreadManager.h"
#include "Resource/Shader/GlobalConstantBuffer.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Component/CameraComponent.h"
#include "Resource/Shader/StructuredBuffer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

float g_DeltaTime;

std::function<bool(HWND, UINT, WPARAM, LPARAM)> CEngine::m_WndProcFunc;

DEFINITION_SINGLE(CEngine)

bool CEngine::m_Loop = true;
bool CEngine::m_EditorMode = false;

CEngine::CEngine()	:
	m_hInst(0),
	m_hWnd(0),
	m_WindowRS{},
	m_ClearColor{},
	m_Render2D(false),
	m_GlobalCBuffer(nullptr),
	m_RandomBuffer(nullptr),
	m_AccTime(0.f),
	m_TimeScale(1.f)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(5242);

	srand((unsigned int)time(0));
	rand();
}

CEngine::~CEngine()
{
	if (m_RandomBuffer)
	{
		m_RandomBuffer->ResetShader(51, (int)EShaderBufferType::All);
	}

	SAFE_DELETE(m_RandomBuffer);

	SAFE_DELETE(m_GlobalCBuffer);

	CSceneManager::DestroyInst();

	CEditorGUIManager::DestroyInst();

	CCollisionManager::DestroyInst();

	CRenderManager::DestroyInst();

	CInput::DestroyInst();

	CThreadManager::DestroyInst();

	CPathManager::DestroyInst();
	CResourceManager::DestroyInst();

	SAFE_DELETE(m_Setting);

	SAFE_DELETE(m_Timer);

	CDevice::DestroyInst();
}

float CEngine::GetFPS() const
{
	return m_Timer->GetFPS();
}

float CEngine::GetDeltaTime() const
{
	return m_Timer->GetDeltaTime();
}

void CEngine::SetCameraAxisX(const Vector3& Axis)
{
	m_GlobalCBuffer->SetCameraAxisX(Axis);
}

void CEngine::SetCameraAxisY(const Vector3& Axis)
{
	m_GlobalCBuffer->SetCameraAxisY(Axis);
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR* Title,
	const TCHAR* ClassName, int IconID, int SmallIconID,
	unsigned int WindowWidth, unsigned int WindowHeight,
	unsigned int DeviceWidth, unsigned int DeviceHeight, bool WindowMode,
	int MenuID)
{
	m_hInst = hInst;
	m_WindowRS.Width = WindowWidth;
	m_WindowRS.Height = WindowHeight;

	Register(ClassName, IconID, SmallIconID, MenuID);

	Create(Title, ClassName);

	m_Setting = new CEngineSetting;

	m_Setting->Init();

	// Device 초기화
	if (!CDevice::GetInst()->Init(m_hWnd, DeviceWidth, DeviceHeight, WindowMode))
		return false;

	// 경로관리자 초기화
	if (!CPathManager::GetInst()->Init())
		return false;

	// 렌더링 관리자 초기화
	if (!CRenderManager::GetInst()->Init())
		return false;

	// Resource 관리자 초기화
	if (!CResourceManager::GetInst()->Init())
		return false;

	CRenderManager::GetInst()->CreateRenderTarget();


	// 충돌 관리자 초기화
	if (!CCollisionManager::GetInst()->Init())
		return false;

	
	// 입력 관리자 초기화
	if (!CInput::GetInst()->Init(m_hInst, m_hWnd))
		return false;

	if (m_EditorMode)
	{
		if (!CEditorGUIManager::GetInst()->Init(m_hWnd))
		{
			return false;
		}
	}

	
	// 스레드 관리자 초기화
	if (!CThreadManager::GetInst()->Init())
		return false;


	// 장면관리자 초기화
	if (!CSceneManager::GetInst()->Init())
		return false;

	m_Timer = new CTimer;

	m_Timer->Init();

	m_GlobalCBuffer = new CGlobalConstantBuffer;

	m_GlobalCBuffer->Init();

	std::vector<int> vecRand;
	vecRand.resize(1024 * 1024);

	for (int i = 0; i < 1024 * 1024; i++)
	{
		short	Num1 = (short)rand();
		short	Num2 = (short)rand();

		int	Random = Num1;
		Random <<= 16;

		Random |= Num2;

		vecRand[i] = Random;
	}

	m_RandomBuffer = new CStructuredBuffer;

	m_RandomBuffer->Init("Random", sizeof(int), 1024 * 1024, 51, true, (int)EShaderBufferType::All);

	m_RandomBuffer->UpdateBuffer(&vecRand[0], 1024 * 1024);

	m_RandomBuffer->SetShader(51, (int)EShaderBufferType::All);

	return true;
}

int CEngine::Run()
{
	// 운영체제가 만들어준 메세지를 얻어오기 위한 구조체이다.
	MSG msg;

	// GetMessage : 메세지 큐에서 메세지를 꺼내오는 함수이다.
	// 단, 메세지 큐가 비어있을 경우 메세지가 들어올때까지 이 함수를 빠져나올 수 없다.
	// 이렇게 멈춰서 대기하고 있는 것을 블로킹 모드 라고 한다.
	// 큐가 비어서 멈춰있는 시간을 윈도우의 데드타임이라고 부른다.
	while (m_Loop)
	{
		// PeekMessage : 이 함수도 메세지 큐에서 메세지를 꺼내오는
		// 함수이다. 단, 이 함수는 메세지 큐가 비어있을 경우 false
		// 를 반환하며 바로 빠져나오게 된다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 메세지큐에서 꺼내온 메세지를 TranslateMessage 함수로 넘겨주면 문자 키인지 
			// F1, 방향키 같은 키인지를 판단해준다.
			// 이러한 키들은 WM_KEYDOWN으로 메세지가 인식이 되고
			// 문자 키는 WM_CHAR로 인식이 된다.
			// 키를 누르면 문자키의 경우 WM_CHAR도 만들어져야하기 때문에 WM_KEYDOWN
			// 이 일어나면 문자키의 경우 WM_CHAR메세지를 추가로 만들어서 메세지 큐에
			// 넣어준다.
			TranslateMessage(&msg);

			// DispatchMessage 함수는 메세지큐에서 꺼내온 메세지를 메세지 처리 함수에
			// 보내준다. WndProc로 보내주는 것이다.
			DispatchMessage(&msg);
		}

		// 메세지 큐가 비어있을 경우 동작된다. 즉, 윈도우의 데드타임시
		// 이 else 구문이 동작되기 때문에 여기에 실제 게임로직을
		// 작성하면 된다.
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	m_Timer->Update();

	float DeltaTime = m_Timer->GetDeltaTime()* m_TimeScale;

	g_DeltaTime = DeltaTime;

	m_AccTime += DeltaTime;

	Resolution	RS = CDevice::GetInst()->GetResolution();

	m_GlobalCBuffer->SetDeltaTime(DeltaTime);
	m_GlobalCBuffer->SetAccTime(m_AccTime);
	m_GlobalCBuffer->SetResolution((float)RS.Width, (float)RS.Height);

	CInput::GetInst()->Update(DeltaTime);

	if (m_EditorMode)
	{
		CEditorGUIManager::GetInst()->Update(DeltaTime);
	}

	CResourceManager::GetInst()->Update();

	Input(DeltaTime);


	if (Update(DeltaTime))
	{
		if (m_EditorMode)
			CEditorGUIManager::GetInst()->Render();
		return;
	}

	if (PostUpdate(DeltaTime))
	{
		if (m_EditorMode)
			CEditorGUIManager::GetInst()->Render();
		return;
	}

	Collision(DeltaTime);
	Render(DeltaTime);
}

void CEngine::Input(float DeltaTime)
{
}

bool CEngine::Update(float DeltaTime)
{
	bool Result = CSceneManager::GetInst()->Update(DeltaTime);

	return Result;
}

bool CEngine::PostUpdate(float DeltaTime)
{
	bool Result = CSceneManager::GetInst()->PostUpdate(DeltaTime);

	return Result;
}

bool CEngine::Collision(float DeltaTime)
{
	return CSceneManager::GetInst()->Collision(DeltaTime);
}

void CEngine::Render(float DeltaTime)
{
	// 카메라의 축을 전달한다.
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	if (Camera)
	{
		m_GlobalCBuffer->SetCameraAxisX(Camera->GetWorldAxis(AXIS_X));
		m_GlobalCBuffer->SetCameraAxisY(Camera->GetWorldAxis(AXIS_Y));
	}

	m_GlobalCBuffer->UpdateBuffer();

	CDevice::GetInst()->ClearRenderTarget(m_ClearColor);
	CDevice::GetInst()->ClearDepthStencil(1.f, 0);

	CDevice::GetInst()->RenderStart();


	// 모든 물체들을 출력한다. 이렇게 하면 백버퍼와 깊이버퍼가 채워진다.
	CRenderManager::GetInst()->Render(DeltaTime);

	if (m_EditorMode)
		CEditorGUIManager::GetInst()->Render();

	// 그려진 백버퍼를 화면에 시연한다.
	CDevice::GetInst()->Flip();

	CInput::GetInst()->SetWheel(0);
}

void CEngine::Register(const TCHAR* ClassName, int IconID, int SmallIconID, int MenuID)
{
	// 레지스터에 등록할 윈도우 클래스 구조체를 만들어준다.
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// 메세지큐에서 꺼내온 메세지를 인자로 전달하며 호출할 함수의 함수 주소를
	// 등록한다.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// 윈도우 인스턴스를 등록한다.
	wcex.hInstance = m_hInst;

	// 실행파일에 사용할 아이콘을 등록한다.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));

	// 마우스 커서 모양을 결정한다.
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	// 메뉴를 사용할지 말지를 결정한다.
	if (MenuID != 0)
		wcex.lpszMenuName = MAKEINTRESOURCEW(MenuID);

	else
		wcex.lpszMenuName = nullptr;

	// 등록할 클래스의 이름을 유니코드 문자열로 만들어서 지정한다.
	// TEXT 매크로는 프로젝트 설정이 유니코드로 되어있을 경우 유니코드 문자열로 만들어지고
	// 멀티바이트로 되어있을 경우 멀티바이트 문자열로 만들어지게 된다.
	wcex.lpszClassName = ClassName;

	// 윈도우창 좌상단에 표시할 작은 아이콘을 등록한다.
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(SmallIconID));

	RegisterClassExW(&wcex);
}

bool CEngine::Create(const TCHAR* Title, const TCHAR* ClassName)
{
	// 윈도우 창을 생성한다.
	// 1번 인자는 윈도우 클래스에 등록한 이름이다.
	// 2번 인자는 타이틀바에 표시할 이름이다.
	// 3번인자는 이 윈도우 창이 어떻게 생성될지를 지정하는 옵션이다.
	// 4, 5번 인자는 이 윈도우 창이 생성될 화면에서의 위치를 지정한다.
	// 픽셀로 지정한다. 예를 들어 1920, 1080 해상도라면 거기에서 원하는 값을 넣어주면
	// 해당 위치에 나오게 된다.
	// 4번은 가로좌표, 5번은 세로좌표로 사용이 된다.
	// 6번, 7번 인자는 윈도우창의 가로, 세로의 크기를 지정한다.
	// 픽셀단위로 지정을 해준다.
	// 8번 인자는 부모윈도우가 있다면 부모윈도우의 핸들을 지정한다.
	// 없으면 nullptr을 지정한다.
	// 9번 인자는 메뉴가 있다면 메뉴 핸들을 넣어주고 없으면 nullptr을 지정한다.
	// 10번 인자는 윈도우 인스턴스를 지정하여 이 윈도우 인스턴스에 속한 윈도우 창을
	// 만들어주게 된다.
	// 윈도우 창을 만들어주고 정상적으로 만들어졌다면 생성된 윈도우 창의 핸들을
	// 반환해준다.
	// HWND 가 윈도우 핸들을 의미한다.
	// 만약 생성이 제대로 안되었다면 0을 반환한다.
	m_hWnd = CreateWindowW(ClassName, Title, WS_OVERLAPPEDWINDOW,
		100, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return false;
	}

	// RECT : 사각형을 표현하기 위해서 지원하는 구조체이다.
	// left, top, right, bottom 값으로 이루어져 있다.
	// 윈도우 크기를 표현하는 Rect 구조체를 하나 만들어준다.
	RECT    rc = { 0, 0, (LONG)m_WindowRS.Width, (LONG)m_WindowRS.Height };

	// 위에서 지정한 크기만큼 클라이언트 영역의 크기로 잡기 위해서
	// 필요한 실제 윈도우의 크기를 얻어온다.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 위에서 얻어온 크기로 윈도우 창의 크기를 변경한다.
	MoveWindow(m_hWnd, 50, 50, rc.right - rc.left,
		rc.bottom - rc.top, TRUE);

	// 윈도우 창을 보여준다. 1번인자에 들어간 핸들의 윈도우 창을 보여줄지 말지를
	// 결정해준다.
	ShowWindow(m_hWnd, SW_SHOW);

	// 이 함수를 호출하여 클라이언트 영역이 제대로 갱신되었다면 0이 아닌 값을 반환하고
	// 갱신이 실패했을 경우 0을 반환한다.
	UpdateWindow(m_hWnd);

	return true;
}

// 이거 안하면 IMGUI 버튼 클릭 안됨.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, 
	LPARAM lParam)
{
	if (m_EditorMode)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return 1;
	}

	if (m_WndProcFunc)
	{
		if (m_WndProcFunc(hWnd, message, wParam, lParam))
			return 1;
	}

	switch (message)
	{
	case WM_MOUSEWHEEL:
	{
		// wParam의 상위 16비트를 얻어온다.
		// 이렇게 얻어오면 휠 방향에 따라 120, -120이 들어온다.
		short Wheel = HIWORD(wParam);
		CInput::GetInst()->SetWheel(Wheel / 120);
	}
		break;
	case WM_DESTROY:
		// 윈도우가 종료될때 들어오는 메세지이다.
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
