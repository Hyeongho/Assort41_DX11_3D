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

	// Device �ʱ�ȭ
	if (!CDevice::GetInst()->Init(m_hWnd, DeviceWidth, DeviceHeight, WindowMode))
		return false;

	// ��ΰ����� �ʱ�ȭ
	if (!CPathManager::GetInst()->Init())
		return false;

	// ������ ������ �ʱ�ȭ
	if (!CRenderManager::GetInst()->Init())
		return false;

	// Resource ������ �ʱ�ȭ
	if (!CResourceManager::GetInst()->Init())
		return false;

	CRenderManager::GetInst()->CreateRenderTarget();


	// �浹 ������ �ʱ�ȭ
	if (!CCollisionManager::GetInst()->Init())
		return false;

	
	// �Է� ������ �ʱ�ȭ
	if (!CInput::GetInst()->Init(m_hInst, m_hWnd))
		return false;

	if (m_EditorMode)
	{
		if (!CEditorGUIManager::GetInst()->Init(m_hWnd))
		{
			return false;
		}
	}

	
	// ������ ������ �ʱ�ȭ
	if (!CThreadManager::GetInst()->Init())
		return false;


	// �������� �ʱ�ȭ
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
	// �ü���� ������� �޼����� ������ ���� ����ü�̴�.
	MSG msg;

	// GetMessage : �޼��� ť���� �޼����� �������� �Լ��̴�.
	// ��, �޼��� ť�� ������� ��� �޼����� ���ö����� �� �Լ��� �������� �� ����.
	// �̷��� ���缭 ����ϰ� �ִ� ���� ���ŷ ��� ��� �Ѵ�.
	// ť�� �� �����ִ� �ð��� �������� ����Ÿ���̶�� �θ���.
	while (m_Loop)
	{
		// PeekMessage : �� �Լ��� �޼��� ť���� �޼����� ��������
		// �Լ��̴�. ��, �� �Լ��� �޼��� ť�� ������� ��� false
		// �� ��ȯ�ϸ� �ٷ� ���������� �ȴ�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// �޼���ť���� ������ �޼����� TranslateMessage �Լ��� �Ѱ��ָ� ���� Ű���� 
			// F1, ����Ű ���� Ű������ �Ǵ����ش�.
			// �̷��� Ű���� WM_KEYDOWN���� �޼����� �ν��� �ǰ�
			// ���� Ű�� WM_CHAR�� �ν��� �ȴ�.
			// Ű�� ������ ����Ű�� ��� WM_CHAR�� ����������ϱ� ������ WM_KEYDOWN
			// �� �Ͼ�� ����Ű�� ��� WM_CHAR�޼����� �߰��� ���� �޼��� ť��
			// �־��ش�.
			TranslateMessage(&msg);

			// DispatchMessage �Լ��� �޼���ť���� ������ �޼����� �޼��� ó�� �Լ���
			// �����ش�. WndProc�� �����ִ� ���̴�.
			DispatchMessage(&msg);
		}

		// �޼��� ť�� ������� ��� ���۵ȴ�. ��, �������� ����Ÿ�ӽ�
		// �� else ������ ���۵Ǳ� ������ ���⿡ ���� ���ӷ�����
		// �ۼ��ϸ� �ȴ�.
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
	// ī�޶��� ���� �����Ѵ�.
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


	// ��� ��ü���� ����Ѵ�. �̷��� �ϸ� ����ۿ� ���̹��۰� ä������.
	CRenderManager::GetInst()->Render(DeltaTime);

	if (m_EditorMode)
		CEditorGUIManager::GetInst()->Render();

	// �׷��� ����۸� ȭ�鿡 �ÿ��Ѵ�.
	CDevice::GetInst()->Flip();

	CInput::GetInst()->SetWheel(0);
}

void CEngine::Register(const TCHAR* ClassName, int IconID, int SmallIconID, int MenuID)
{
	// �������Ϳ� ����� ������ Ŭ���� ����ü�� ������ش�.
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// �޼���ť���� ������ �޼����� ���ڷ� �����ϸ� ȣ���� �Լ��� �Լ� �ּҸ�
	// ����Ѵ�.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// ������ �ν��Ͻ��� ����Ѵ�.
	wcex.hInstance = m_hInst;

	// �������Ͽ� ����� �������� ����Ѵ�.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));

	// ���콺 Ŀ�� ����� �����Ѵ�.
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	// �޴��� ������� ������ �����Ѵ�.
	if (MenuID != 0)
		wcex.lpszMenuName = MAKEINTRESOURCEW(MenuID);

	else
		wcex.lpszMenuName = nullptr;

	// ����� Ŭ������ �̸��� �����ڵ� ���ڿ��� ���� �����Ѵ�.
	// TEXT ��ũ�δ� ������Ʈ ������ �����ڵ�� �Ǿ����� ��� �����ڵ� ���ڿ��� ���������
	// ��Ƽ����Ʈ�� �Ǿ����� ��� ��Ƽ����Ʈ ���ڿ��� ��������� �ȴ�.
	wcex.lpszClassName = ClassName;

	// ������â �»�ܿ� ǥ���� ���� �������� ����Ѵ�.
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(SmallIconID));

	RegisterClassExW(&wcex);
}

bool CEngine::Create(const TCHAR* Title, const TCHAR* ClassName)
{
	// ������ â�� �����Ѵ�.
	// 1�� ���ڴ� ������ Ŭ������ ����� �̸��̴�.
	// 2�� ���ڴ� Ÿ��Ʋ�ٿ� ǥ���� �̸��̴�.
	// 3�����ڴ� �� ������ â�� ��� ���������� �����ϴ� �ɼ��̴�.
	// 4, 5�� ���ڴ� �� ������ â�� ������ ȭ�鿡���� ��ġ�� �����Ѵ�.
	// �ȼ��� �����Ѵ�. ���� ��� 1920, 1080 �ػ󵵶�� �ű⿡�� ���ϴ� ���� �־��ָ�
	// �ش� ��ġ�� ������ �ȴ�.
	// 4���� ������ǥ, 5���� ������ǥ�� ����� �ȴ�.
	// 6��, 7�� ���ڴ� ������â�� ����, ������ ũ�⸦ �����Ѵ�.
	// �ȼ������� ������ ���ش�.
	// 8�� ���ڴ� �θ������찡 �ִٸ� �θ��������� �ڵ��� �����Ѵ�.
	// ������ nullptr�� �����Ѵ�.
	// 9�� ���ڴ� �޴��� �ִٸ� �޴� �ڵ��� �־��ְ� ������ nullptr�� �����Ѵ�.
	// 10�� ���ڴ� ������ �ν��Ͻ��� �����Ͽ� �� ������ �ν��Ͻ��� ���� ������ â��
	// ������ְ� �ȴ�.
	// ������ â�� ������ְ� ���������� ��������ٸ� ������ ������ â�� �ڵ���
	// ��ȯ���ش�.
	// HWND �� ������ �ڵ��� �ǹ��Ѵ�.
	// ���� ������ ����� �ȵǾ��ٸ� 0�� ��ȯ�Ѵ�.
	m_hWnd = CreateWindowW(ClassName, Title, WS_OVERLAPPEDWINDOW,
		100, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return false;
	}

	// RECT : �簢���� ǥ���ϱ� ���ؼ� �����ϴ� ����ü�̴�.
	// left, top, right, bottom ������ �̷���� �ִ�.
	// ������ ũ�⸦ ǥ���ϴ� Rect ����ü�� �ϳ� ������ش�.
	RECT    rc = { 0, 0, (LONG)m_WindowRS.Width, (LONG)m_WindowRS.Height };

	// ������ ������ ũ�⸸ŭ Ŭ���̾�Ʈ ������ ũ��� ��� ���ؼ�
	// �ʿ��� ���� �������� ũ�⸦ ���´�.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// ������ ���� ũ��� ������ â�� ũ�⸦ �����Ѵ�.
	MoveWindow(m_hWnd, 50, 50, rc.right - rc.left,
		rc.bottom - rc.top, TRUE);

	// ������ â�� �����ش�. 1�����ڿ� �� �ڵ��� ������ â�� �������� ������
	// �������ش�.
	ShowWindow(m_hWnd, SW_SHOW);

	// �� �Լ��� ȣ���Ͽ� Ŭ���̾�Ʈ ������ ����� ���ŵǾ��ٸ� 0�� �ƴ� ���� ��ȯ�ϰ�
	// ������ �������� ��� 0�� ��ȯ�Ѵ�.
	UpdateWindow(m_hWnd);

	return true;
}

// �̰� ���ϸ� IMGUI ��ư Ŭ�� �ȵ�.
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
		// wParam�� ���� 16��Ʈ�� ���´�.
		// �̷��� ������ �� ���⿡ ���� 120, -120�� ���´�.
		short Wheel = HIWORD(wParam);
		CInput::GetInst()->SetWheel(Wheel / 120);
	}
		break;
	case WM_DESTROY:
		// �����찡 ����ɶ� ������ �޼����̴�.
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
