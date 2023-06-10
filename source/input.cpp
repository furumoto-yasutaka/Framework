/*******************************************************************************
*
*	�^�C�g���F	�L�[���͗p�ÓI�N���X
*	�t�@�C���F	input.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "input.h"

void Input::Init(HINSTANCE hInstance)
{
	//ShowCursor(false);

	HRESULT hr;
	HWND hWnd = GetWindow();

	// DirectInput�I�u�W�F�N�g�̍쐬
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
							IID_IDirectInput8, (void**)&m_DInput, NULL);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			TEXT("DirectInput�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���"),
			TEXT("input:Error"),
			MB_OK | MB_ICONERROR);
		return;
	}

	InitKeyboard(hWnd);
	InitMouse(hWnd);
	InitGamePad();
}

void Input::Uninit()
{
	UninitKeyboard();
	UninitMouse();
	UninitGamePad();

	if (m_DInput)
	{
		m_DInput->Release();
		m_DInput = NULL;
	}
}

void Input::Update()
{
	UpdateKeyboard();
	UpdateMouse();
	UpdateGamePad();
}


void Input::InitKeyboard(HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_DInput->CreateDevice(GUID_SysKeyboard, &m_DIDevKeyboard, NULL);
	if (FAILED(hr) || m_DIDevKeyboard == NULL)
	{
		MessageBox(hWnd,
			TEXT("�L�[�{�[�h��������܂���ł���"),
			TEXT("input:Error"),
			MB_OK | MB_ICONERROR);
		return;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_DIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			TEXT("�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł���"),
			TEXT("input:Error"),
			MB_OK | MB_ICONERROR);
		return;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_DIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			TEXT("�L�[�{�[�h�̋������[�h�ݒ�Ɏ��s���܂���"),
			TEXT("input:Error"),
			MB_OK | MB_ICONERROR);
		return;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	hr = m_DIDevKeyboard->Acquire();
}
void Input::InitMouse(HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�쐬
	hr = m_DInput->CreateDevice(GUID_SysMouse, &m_DIDevMouse, NULL);
	if (FAILED(hr) || m_DIDevMouse == NULL)
	{
		MessageBox(hWnd,
			TEXT("�}�E�X��������܂���ł���"),
			TEXT("input:Error"),
			MB_OK | MB_ICONERROR);
		return;
	}

	// �f�[�^�t�H�[�}�b�g�ݒ�
	hr = m_DIDevMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			TEXT("�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł���"),
			TEXT("input:Error"),
			MB_OK | MB_ICONERROR);
		return;
	}

	// ���̃A�v���Ƌ������[�h�ɐݒ�
	hr = m_DIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			TEXT("�}�E�X�̋������[�h�ݒ�Ɏ��s���܂���"),
			TEXT("input:Error"),
			MB_OK | MB_ICONERROR);
		return;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;
	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	hr = m_DIDevMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(hr))
	{
		MessageBox(hWnd,
			TEXT("�}�E�X�̐ݒ�Ɏ��s���܂���"),
			TEXT("input:Error"),
			MB_OK | MB_ICONERROR);
		return;
	}

	// �A�N�Z�X���𓾂�
	m_DIDevMouse->Acquire();
}
void Input::InitGamePad()
{
	XInputEnable(true);
}


void Input::UninitKeyboard()
{
	m_DIDevKeyboard->Unacquire();
	m_DIDevKeyboard->Release();
	m_DIDevKeyboard = NULL;
}
void Input::UninitMouse()
{
	m_DIDevMouse->Unacquire();
	m_DIDevMouse->Release();
	m_DIDevMouse = NULL;
}
void Input::UninitGamePad()
{
	//�o�C�u���[�V������~
	for (DWORD i = 0; i < m_GAME_PAD_MAX; i++)
	{
		XInputSetState(i, &m_GamePadVibration[i]);
	}

	XInputEnable(false);
}


void Input::UpdateKeyboard()
{
	// �O��̃f�[�^��ۑ�
	memcpy(m_OldKeyState, m_KeyState, m_NUM_KEY_MAX);

	HRESULT hr;
	// �f�o�C�X����f�[�^���擾
	hr = m_DIDevKeyboard->GetDeviceState(sizeof(m_KeyState), m_KeyState);
	if (FAILED(hr))
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X���̎擾���ēx���݂�
		m_DIDevKeyboard->Acquire();
	}
}
void Input::UpdateMouse()
{
	// �O��̒l�ۑ�
	m_OldMouseState = m_MouseState;

	HRESULT hr;
	// �f�[�^�擾
	hr = m_DIDevMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
	if (FAILED(hr))
	{
		// �}�E�X�ւ̃A�N�Z�X���̎擾���ēx���݂�
		hr = m_DIDevMouse->Acquire();
	}
}
void Input::UpdateGamePad()
{
	for (DWORD i = 0; i < m_GAME_PAD_MAX; i++)
	{
		XInputSetState(i, &m_GamePadVibration[i]);

		m_OldGamePadState[i] = m_GamePadState[i];

		//����FERROR_SUCCESS�@0
		//�ڑ������FERROR_DEVICE_NOT_CONNECTED 1167
		DWORD result;
		result = XInputGetState(i, &m_GamePadState[i]);

		//L�X�e�B�b�N�̃f�b�h�]�[���ݒ�
		if (fabs(m_GamePadState[i].Gamepad.sThumbLX) < m_STICK_DEADZONE &&
			fabs(m_GamePadState[i].Gamepad.sThumbLY) < m_STICK_DEADZONE)
		{
			m_GamePadState[i].Gamepad.sThumbLX = 0;
			m_GamePadState[i].Gamepad.sThumbLY = 0;
		}
		//R�X�e�B�b�N�̃f�b�h�]�[���ݒ�
		if (fabs(m_GamePadState[i].Gamepad.sThumbRX) < m_STICK_DEADZONE &&
			fabs(m_GamePadState[i].Gamepad.sThumbRY) < m_STICK_DEADZONE)
		{
			m_GamePadState[i].Gamepad.sThumbRX = 0;
			m_GamePadState[i].Gamepad.sThumbRY = 0;
		}

		if (m_GamePadState[i].Gamepad.bLeftTrigger > m_TRIGGER_DEADZONE)
		{
			m_GamePadState[i].Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_TRIGGER;
		}
		if (m_GamePadState[i].Gamepad.bRightTrigger > m_TRIGGER_DEADZONE)
		{
			m_GamePadState[i].Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_TRIGGER;
		}
	}
}

//-------------------------------------------------------------------------------------- �L�[�{�[�h

/*******************************************************************************
*	�L�[�{�[�h�@�L�[�������ꂽ�u��
*******************************************************************************/
bool Input::GetKeyboardTrigger(BYTE keyCode)
{
	return (m_KeyState[keyCode] & 0x80) && !(m_OldKeyState[keyCode] & 0x80);
}
/*******************************************************************************
*	�L�[�{�[�h�@�L�[��������Ă��邩
*******************************************************************************/
bool Input::GetKeyboardPress(BYTE keyCode)
{
	return (m_KeyState[keyCode] & 0x80);
}
/*******************************************************************************
*	�L�[�{�[�h�@�L�[�����ꂽ�u��
*******************************************************************************/
bool Input::GetKeyboardRelease(BYTE keyCode)
{
	return !(m_KeyState[keyCode] & 0x80) && (m_OldKeyState[keyCode] & 0x80);
}



//-------------------------------------------------------------------------------------- �}�E�X

/*******************************************************************************
*	�}�E�X�@�{�^���������ꂽ�u��
*******************************************************************************/
bool Input::GetMouseTrigger(MouseButton button)
{
	return (m_MouseState.rgbButtons[(int)button] & 0x80) && !(m_OldMouseState.rgbButtons[(int)button] & 0x80);
}
/*******************************************************************************
*	�}�E�X�@�{�^����������Ă��邩
*******************************************************************************/
bool Input::GetMousePress(MouseButton button)
{
	return (m_MouseState.rgbButtons[(int)button] & 0x80);
}
/*******************************************************************************
*	�}�E�X�@�{�^�������ꂽ�u��
*******************************************************************************/
bool Input::GetMouseRelease(MouseButton button)
{
	return !(m_MouseState.rgbButtons[(int)button] & 0x80) && (m_OldMouseState.rgbButtons[(int)button] & 0x80);
}

/*******************************************************************************
*	�}�E�X�@���E�̈ړ���
*******************************************************************************/
long Input::GetMouseMoveX()
{
	return m_MouseState.lX;
}
/*******************************************************************************
*	�}�E�X�@�㉺�̈ړ���
*******************************************************************************/
long Input::GetMouseMoveY()
{
	return m_MouseState.lY;
}
/*******************************************************************************
*	�}�E�X�@�}�E�X�z�C�[��
*******************************************************************************/
long Input::GetMouseMoveZ()
{
	return m_MouseState.lZ;
}

/*******************************************************************************
*	�}�E�X�@�J�[�\���ʒu(�N���C�A���g���W)
*******************************************************************************/
D3DXVECTOR2 Input::GetMouseCursorClient()
{
	POINT p;
	D3DXVECTOR2 windowSize = GetWindowSize();
	GetCursorPos(&p);
	ScreenToClient(GetWindow(), &p);
	D3DXVECTOR2 pos = { (float)p.x, (float)p.y };
	pos.x *= (float)WINDOW_RESOLUTION_WIDTH / windowSize.x;
	pos.y *= (float)WINDOW_RESOLUTION_HEIGHT / windowSize.y;
	return pos;
}

/*******************************************************************************
*	�}�E�X�@�J�[�\���ʒu(�X�N���[�����W)
*******************************************************************************/
D3DXVECTOR2 Input::GetMouseCursorScreen()
{
	POINT p;
	GetCursorPos(&p);
	D3DXVECTOR2 pos = { (float)p.x, (float)p.y };
	return pos;
}



//-------------------------------------------------------------------------------------- �R���g���[���[

/*******************************************************************************
*	�R���g���[���[�@�{�^���������ꂽ�u��
*******************************************************************************/
bool Input::GetGamePadButtonTrigger(DWORD button, int padNo)
{
	return (button & m_GamePadState[padNo].Gamepad.wButtons) && !(button & m_OldGamePadState[padNo].Gamepad.wButtons);
}
/*******************************************************************************
*	�R���g���[���[�@�{�^����������Ă��邩
*******************************************************************************/
bool Input::GetGamePadButtonPress(DWORD button, int padNo)
{
	return (button & m_GamePadState[padNo].Gamepad.wButtons);
}
/*******************************************************************************
*	�R���g���[���[�@�{�^�������ꂽ�u��
*******************************************************************************/
bool Input::GetGamePadButtonRelease(DWORD button, int padNo)
{
	return !(button & m_GamePadState[padNo].Gamepad.wButtons) && (button & m_OldGamePadState[padNo].Gamepad.wButtons);
}

/*******************************************************************************
*	�R���g���[���[�@���X�e�B�b�N(���E)
*******************************************************************************/
float Input::GetGamePadStickLeftX(int padNo)
{
	return m_GamePadState[padNo].Gamepad.sThumbLX * m_STICK_DIV;
}
/*******************************************************************************
*	�R���g���[���[�@���X�e�B�b�N(�㉺)
*******************************************************************************/
float Input::GetGamePadStickLeftY(int padNo)
{
	return m_GamePadState[padNo].Gamepad.sThumbLY * m_STICK_DIV;
}
/*******************************************************************************
*	�R���g���[���[�@�E�X�e�B�b�N(���E)
*******************************************************************************/
float Input::GetGamePadStickRightX(int padNo)
{
	return m_GamePadState[padNo].Gamepad.sThumbRX * m_STICK_DIV;
}
/*******************************************************************************
*	�R���g���[���[�@�E�X�e�B�b�N(�㉺)
*******************************************************************************/
float Input::GetGamePadStickRightY(int padNo)
{
	return m_GamePadState[padNo].Gamepad.sThumbRY * m_STICK_DIV;
}

/*******************************************************************************
*	�R���g���[���[�@���g���K�[
*******************************************************************************/
float Input::GetGamePadLeftTrigger(int padNo)
{
	return m_GamePadState[padNo].Gamepad.bLeftTrigger * m_TRIGGER_DIV;
}
/*******************************************************************************
*	�R���g���[���[�@�E�g���K�[
*******************************************************************************/
float Input::GetGamePadRightTrigger(int padNo)
{
	return m_GamePadState[padNo].Gamepad.bRightTrigger * m_TRIGGER_DIV;
}

/*******************************************************************************
*	�R���g���[���[�@���o�C�u���[�V�����ݒ�
*******************************************************************************/
void Input::SetGamePadLeftVibration(int speed, int padNo)
{
	speed %= 256;
	m_GamePadVibration[padNo].wLeftMotorSpeed = ((speed + 1) * 256) - 1;
}
/*******************************************************************************
*	�R���g���[���[�@�E�o�C�u���[�V�����ݒ�
*******************************************************************************/
void Input::SetGamePadRightVibration(int speed, int padNo)
{
	speed %= 256;
	m_GamePadVibration[padNo].wRightMotorSpeed = ((speed + 1) * 256) - 1;
}
/*******************************************************************************
*	�R���g���[���[�@���T�C�h�o�C�u���[�V�����ݒ�
*******************************************************************************/
void Input::SetGamePadVibration(int speed, int padNo)
{
	speed %= 256;
	m_GamePadVibration[padNo].wLeftMotorSpeed = ((speed + 1) * 256) - 1;
	m_GamePadVibration[padNo].wRightMotorSpeed = ((speed + 1) * 256) - 1;
}
/*******************************************************************************
*	�R���g���[���[�@�o�C�u���[�V������~
*******************************************************************************/
void Input::StopGamePadVibration(int padNo)
{
	m_GamePadVibration[padNo].wLeftMotorSpeed = 0;
	m_GamePadVibration[padNo].wRightMotorSpeed = 0;
}
