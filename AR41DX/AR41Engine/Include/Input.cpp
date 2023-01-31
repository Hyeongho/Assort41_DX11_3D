#include "Input.h"
#include "Resource/ResourceManager.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Scene/SceneCollision.h"
#include "Component/CameraComponent.h"

#pragma comment(lib, "dinput8.lib")

DEFINITION_SINGLE(CInput)

CInput::CInput() : m_MouseLDown(false), m_MouseLPush(false), m_MouseLUp(false), m_InputSystem(InputSystem_Type::DInput), m_Input(nullptr),
	m_Keyboard(nullptr), m_Mouse(nullptr), m_KeyArray{}, m_MouseState{}, m_CollisionWidget(false), m_Wheel(0)
{
}

CInput::~CInput()
{
	{
		auto iter = m_mapKeyState.begin();
		auto iterEnd = m_mapKeyState.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(iter->second);
		}

		m_mapKeyState.clear();
	}

	{
		auto iter = m_mapBindKey.begin();
		auto iterEnd = m_mapBindKey.end();

		for (; iter != iterEnd; iter++)
		{
			for (int i = 0; i < (int)Input_Type::End; i++)
			{
				size_t	Size = iter->second->vecFunction[i].size();

				for (size_t j = 0; j < Size; j++)
				{
					SAFE_DELETE(iter->second->vecFunction[i][j]);
				}
			}

			SAFE_DELETE(iter->second);
		}

		m_mapBindKey.clear();
	}

	SAFE_RELEASE(m_Keyboard);
	SAFE_RELEASE(m_Mouse);
	SAFE_RELEASE(m_Input);
}

void CInput::ComputeWorldMousePos(const Vector2& CameraPos)
{
	m_MouseWorldPos = m_MousePos + CameraPos;
}

bool CInput::InitDirectInput()
{
	if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_Keyboard, nullptr)))
	{
		return false;
	}

	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	if (FAILED(m_Input->CreateDevice(GUID_SysMouse, &m_Mouse, nullptr)))
	{
		return false;
	}

	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}

	m_Keyboard->Acquire();
	m_Mouse->Acquire();

	return true;
}

void CInput::ReadDirectInputKeyboard()
{
	HRESULT	result = m_Keyboard->GetDeviceState(256, m_KeyArray);

	/*if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_Keyboard->Acquire();
		}
	}*/
}

void CInput::ReadDirectInputMouse()
{
	HRESULT	result = m_Mouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	/*if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_Mouse->Acquire();
		}
	}*/
}

bool CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	m_hWnd = hWnd;

	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Input, nullptr)))
	{
		m_InputSystem = InputSystem_Type::Window;
	}

	if (m_InputSystem == InputSystem_Type::DInput)
	{
		if (!InitDirectInput())
		{
			return false;
		}
	}

	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;

	m_ShowCursor = true;

	return true;
}

void CInput::Update(float DeltaTime)
{
	if (m_InputSystem == InputSystem_Type::DInput)
	{
		ReadDirectInputKeyboard();
		ReadDirectInputMouse();
	}

	UpdateMouse(DeltaTime);

	// 마우스와 Widget과의 충돌처리를 진행한다.
	m_CollisionWidget = CSceneManager::GetInst()->GetScene()->GetCollisionManager()->CollisionWidget();

	UpdateKeyState(DeltaTime);

	UpdateBindKey(DeltaTime);
}

void CInput::PostUpdate(float DeltaTime)
{
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT ptMouse;

	// 스크린 좌표로 나온다.
	GetCursorPos(&ptMouse);

	// 스크린 좌표를 윈도우 좌표로 변경한다.
	ScreenToClient(m_hWnd, &ptMouse);

	RECT WindowRC;

	GetClientRect(m_hWnd, &WindowRC);

	Vector2	ResolutionRatio = CDevice::GetInst()->GetResolutionRatio();

	Vector2	MousePos;

	MousePos.x = (float)ptMouse.x * ResolutionRatio.x;
	MousePos.y = (float)(WindowRC.bottom - WindowRC.top - ptMouse.y) * ResolutionRatio.y;

	m_MouseMove = MousePos - m_MousePos;

	m_MousePos = MousePos;

	// WorldPosition을 만들어준다. 마우스의 위치에 카메라의 위치를 더한다.
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Vector3 CameraPos = Camera->GetWorldPos();

	m_MouseWorldPos = m_MousePos + Vector2(CameraPos.x, CameraPos.y);


	/*RECT	rc = {};
	GetClientRect(m_hWnd, &rc);

	if (rc.left <= ptMouse.x && ptMouse.x <= rc.right &&
		rc.top <= ptMouse.y && ptMouse.y <= rc.bottom)
	{
		if (m_ShowCursor)
		{
			m_ShowCursor = false;
			ShowCursor(FALSE);
		}
	}

	else
	{
		if (!m_ShowCursor)
		{
			m_ShowCursor = true;
			ShowCursor(TRUE);
		}
	}*/
}

void CInput::UpdateKeyState(float DeltaTime)
{
	switch (m_InputSystem)
	{
	case InputSystem_Type::DInput:
		if (m_KeyArray[DIK_LCONTROL] & 0x80)
		{
			m_Ctrl = true;
		}

		else
		{
			m_Ctrl = false;
		}

		if (m_KeyArray[DIK_LALT] & 0x80)
		{
			m_Alt = true;
		}

		else
		{
			m_Alt = false;
		}

		if (m_KeyArray[DIK_LSHIFT] & 0x80)
		{
			m_Shift = true;
		}

		else
		{
			m_Shift = false;
		}


		if (m_MouseState.rgbButtons[0] & 0x80)
		{
			if (!m_MouseLDown && !m_MouseLPush)
			{
				m_MouseLDown = true;
				m_MouseLPush = true;
			}

			else
			{
				m_MouseLDown = false;
			}
		}

		else if (m_MouseLPush)
		{
			m_MouseLDown = false;
			m_MouseLPush = false;
			m_MouseLUp = true;
		}

		else if (m_MouseLUp)
		{
			m_MouseLUp = false;
		}

		break;
	case InputSystem_Type::Window:
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			m_Ctrl = true;
		}

		else
		{
			m_Ctrl = false;
		}

		if (GetAsyncKeyState(VK_MENU) & 0x8000)
		{
			m_Alt = true;
		}

		else
		{
			m_Alt = false;
		}

		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			m_Shift = true;
		}

		else
		{
			m_Shift = false;
		}


		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!m_MouseLDown && !m_MouseLPush)
			{
				m_MouseLDown = true;
				m_MouseLPush = true;
			}

			else
			{
				m_MouseLDown = false;
			}
		}

		else if (m_MouseLPush)
		{
			m_MouseLDown = false;
			m_MouseLPush = false;
			m_MouseLUp = true;
		}

		else if (m_MouseLUp)
		{
			m_MouseLUp = false;
		}
		break;
	}

	auto iter = m_mapKeyState.begin();
	auto iterEnd = m_mapKeyState.end();

	for (; iter != iterEnd; iter++)
	{
		bool	KeyPush = false;

		switch (m_InputSystem)
		{
		case InputSystem_Type::DInput:
			switch (iter->second->key)
			{
			case DIK_MOUSELBUTTON:
				if (m_MouseState.rgbButtons[0] & 0x80)
				{
					KeyPush = true;
				}
				break;
			case DIK_MOUSERBUTTON:
				if (m_MouseState.rgbButtons[1] & 0x80)
				{
					KeyPush = true;
				}
				break;
			case DIK_MOUSEWHEEL:
				break;
			default:	// 일반 키보드 키일때
				if (m_KeyArray[iter->second->key] & 0x80)
				{
					KeyPush = true;
				}
				break;
			}
			break;
		case InputSystem_Type::Window:
			if (GetAsyncKeyState(iter->second->key) & 0x8000)
			{
				KeyPush = true;
			}
			break;
		}

		

		// 키를 눌렀을 경우
		if (KeyPush)
		{
			// Down과 Push 모두 false라면 이 키를 지금 누른 것이다.
			// 그러므로 둘다 true로 변경한다.
			if (!iter->second->Down && !iter->second->Push)
			{
				iter->second->Down = true;
				iter->second->Push = true;
			}

			// Down과 Push 둘중 하나라도 true라면 Down은 false가
			// 되어야 한다. Push는 이미 위에서 true로 변경되었다.
			else
				iter->second->Down = false;
		}

		// 키가 안눌러졌을 경우 Push가 true라면
		// 이전 프레임에 키를 누르고 있다가 지금 떨어졌다는 것이다.
		else if (iter->second->Push)
		{
			iter->second->Up = true;
			iter->second->Push = false;
			iter->second->Down = false;
		}

		else if (iter->second->Up)
			iter->second->Up = false;
	}
}

void CInput::UpdateBindKey(float DeltaTime)
{
	auto iter = m_mapBindKey.begin();
	auto iterEnd = m_mapBindKey.end();

	for (; iter != iterEnd; iter++)
	{
		if (iter->second->key->Down &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t Size = iter->second->vecFunction[(int)Input_Type::Down].size();

			for (size_t i = 0; i < Size; i++)
			{
				iter->second->vecFunction[(int)Input_Type::Down][i]->func();
			}
		}

		if (iter->second->key->Push &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t Size = iter->second->vecFunction[(int)Input_Type::Push].size();

			for (size_t i = 0; i < Size; i++)
			{
				iter->second->vecFunction[(int)Input_Type::Push][i]->func();
			}
		}

		if (iter->second->key->Up &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			size_t	Size = iter->second->vecFunction[(int)Input_Type::Up].size();

			for (size_t i = 0; i < Size; ++i)
			{
				iter->second->vecFunction[(int)Input_Type::Up][i]->func();
			}
		}
	}
}

void CInput::SetKeyCtrl(const std::string& Name, bool Ctrl)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
	{
		return;
	}

	Key->Ctrl = Ctrl;
}

void CInput::SetKeyAlt(const std::string& Name, bool Alt)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
	{
		return;
	}

	Key->Alt = Alt;
}

void CInput::SetKeyShift(const std::string& Name, bool Shift)
{
	BindKey* Key = FindBindKey(Name);

	if (!Key)
	{
		return;
	}

	Key->Shift = Shift;
}

KeyState* CInput::FindKeyState(unsigned char Key)
{
	auto iter = m_mapKeyState.find(Key);

	// 못찾았을 경우
	if (iter == m_mapKeyState.end())
	{
		return nullptr;
	}

	// iter->first : key
	// iter->second : value
	return iter->second;
}

BindKey* CInput::FindBindKey(const std::string& Name)
{
	auto iter = m_mapBindKey.find(Name);

	// 못찾았을 경우
	if (iter == m_mapBindKey.end())
	{
		return nullptr;
	}

	// iter->first : key
	// iter->second : value
	return iter->second;
}

bool CInput::AddBindKey(const std::string& Name,
	unsigned char Key)
{
	// 같은 이름으로 BindKey가 등록되어 있을 경우
	if (FindBindKey(Name))
	{
		return false;
	}

	BindKey* NewKey = new BindKey;

	// 해당 키로 이미 KeyState가 등록되어 있는지를 찾는다.
	KeyState* State = FindKeyState(Key);

	// 등록이 안되어 있을 경우 KeyState를 만들어서 등록한다.
	if (!State)
	{
		State = new KeyState;

		State->key = ConvertKey(Key);

		m_mapKeyState.insert(std::make_pair(Key, State));
	}

	NewKey->key = State;
	NewKey->Name = Name;

	m_mapBindKey.insert(std::make_pair(Name, NewKey));

	return true;
}

void CInput::ClearCallback()
{
	auto iter = m_mapBindKey.begin();
	auto iterEnd = m_mapBindKey.end();

	for (; iter != iterEnd; iter++)
	{
		for (int i = 0; i < (int)Input_Type::End; ++i)
		{
			size_t	Size = iter->second->vecFunction[i].size();

			for (size_t j = 0; j < Size; ++j)
			{
				SAFE_DELETE(iter->second->vecFunction[i][j]);
			}

			iter->second->vecFunction[i].clear();
		}
	}
}

void CInput::ClearCallback(CScene* Scene)
{
	auto	iter = m_mapBindKey.begin();
	auto	iterEnd = m_mapBindKey.end();

	for (; iter != iterEnd; iter++)
	{
		for (int i = 0; i < (int)Input_Type::End; ++i)
		{
			auto iter1 = iter->second->vecFunction[i].begin();
			auto iter1End = iter->second->vecFunction[i].end();

			for (; iter1 != iter1End;)
			{
				if ((*iter1)->Scene == Scene)
				{
					SAFE_DELETE((*iter1));
					iter1 = iter->second->vecFunction[i].erase(iter1);
					iter1End = iter->second->vecFunction[i].end();
					continue;
				}

				iter1++;
			}
		}
	}
}

unsigned char CInput::ConvertKey(unsigned char Key)
{
	if (m_InputSystem == InputSystem_Type::DInput)
	{
		switch (Key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_MBUTTON:
			return DIK_MOUSEWHEEL;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
			//case VK_RETURN:		
			//return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}

		return 0xff;
	}

	return Key;
}
