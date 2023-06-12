/*******************************************************************************
*
*	�^�C�g���F	���C��
*	�t�@�C���F	main.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include <time.h>

#ifdef _DEBUG
#include "debugManager.h"
#include "imgui_impl_win32.h"
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#ifdef _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application::Init(hInstance, nCmdShow);
	while (!Application::m_EndExec)
	{
		Application::Update();
	}
	Application::Uninit();

	return (int)Application::m_Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}
#endif

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			// �I��
			DestroyWindow(hWnd);
			break; 
		case VK_F11:
			Application::m_IsFullScreen = !Application::m_IsFullScreen;
			Renderer::GetSwapChain()->SetFullscreenState(Application::m_IsFullScreen, NULL);
			break;
		default: break;
		}
		break;
	default: break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Application::Init(HINSTANCE hInstance, int nCmdShow)
{
	//�����_���V�[�h�̏�����
	srand((unsigned int)time(NULL));

	// �E�B���h�E�N���X�쐬
	m_Wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		m_CLASS_NAME,
		NULL
	};

	// �E�B���h�E�쐬�ׁ̈A�E�B���h�E�N���X��o�^
	RegisterClassEx(&m_Wcex);

	// �E�B���h�E�쐬
	m_Window = CreateWindowEx(0,
		m_CLASS_NAME,
		m_WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		(int)(GetSystemMetrics(SM_CXSCREEN) * ((1.0f - Application::m_WINDOW_RATE.x) * 0.5f)),		// �E�B���h�E�ʒu(X)
		(int)(GetSystemMetrics(SM_CYSCREEN) * ((1.0f - Application::m_WINDOW_RATE.y) * 0.33f)),	// �E�B���h�E�ʒu(Y)
		(int)(Application::m_WINDOW_RESOLUTION.x + GetSystemMetrics(SM_CXDLGFRAME) * 2),									// �E�B���h�E�T�C�Y(X)
		(int)(Application::m_WINDOW_RESOLUTION.y + GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),	// �E�B���h�E�T�C�Y(Y)
		NULL,
		NULL,
		hInstance,
		NULL);

	//ShowCursor(g_IsMoveCursor);

	switch (m_Mode)
	{
	case Application::Mode::Scene:
		// �Q�[����������
		Manager::Init(hInstance);
		break;
	case Application::Mode::AnimationEditor:
		break;
	default: break;
	}

	// �E�B���h�E��\��
	ShowWindow(m_Window, nCmdShow);
	UpdateWindow(m_Window);

	// �^�C�}�[�̉𑜓x���w��(�~���b)
	timeBeginPeriod(1);

	// �v���p�ϐ�������
	m_ExecLastTime = timeGetTime();
	m_CurrentTime = 0;

#ifdef _DEBUG
	strcpy(m_DebugStr, m_WINDOW_NAME);
	m_FpsLastTime = m_ExecLastTime;
	m_FrameCount = 0;
#endif
}

void Application::Uninit()
{
	timeEndPeriod(1);

	UnregisterClass(m_CLASS_NAME, m_Wcex.hInstance);

	Manager::Uninit();
	_CrtDumpMemoryLeaks();
}

void Application::Update()
{
	if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
	{
		if (m_Msg.message == WM_QUIT)
		{
			m_EndExec = true;
		}
		else
		{
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
	}
	else //if (!g_IsSleep)
	{
		// �o�ߎ��Ԃ��X�V
		m_CurrentTime = timeGetTime();

		// 1/60�b���o�߂�����X�V�E�`������s����
		if ((m_CurrentTime - m_ExecLastTime) >= (1000.0f / 60))
		{
			m_ExecLastTime = m_CurrentTime;
#ifdef _DEBUG
			m_FrameCount++;
#endif
			//if (!g_IsMoveCursor)
			//{
			//	D3DXVECTOR2 pos = GetWindowPosition();
			//	D3DXVECTOR2 size = GetWindowSize();
			//	SetCursorPos((int)(pos.x + size.x * 0.5f), (int)(pos.y + size.y * 0.5f));
			//}
			// �X�V
			Manager::Update();
			// �`��
			Manager::Draw();
			// �V�[���J�ڂ��s�����m�F
			Manager::CheckScene();
		}
#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������	
		if ((m_CurrentTime - m_FpsLastTime) >= 1000.0f)
		{
			m_CountFPS = m_FrameCount;
			m_FpsLastTime = m_CurrentTime;
			m_FrameCount = 0;
			wsprintf(m_DebugStr, m_WINDOW_NAME);
			wsprintf(&m_DebugStr[strlen(m_DebugStr)], " FPS:%d", m_CountFPS);
			SetWindowText(m_Window, m_DebugStr);
		}
#endif
	}
}

/*******************************************************************************
*	�E�B���h�E�擾
*******************************************************************************/
HWND Application::GetWindow()
{
	return m_Window;
}

/*******************************************************************************
*	�E�B���h�E�T�C�Y�擾
*******************************************************************************/
D3DXVECTOR2 Application::GetWindowSize()
{
	RECT r;
	GetWindowRect(m_Window, &r);
	D3DXVECTOR2 s = { (float)(r.right - r.left - (GetSystemMetrics(SM_CXDLGFRAME) * 2)),
				(float)(r.bottom - r.top - (GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION))) };
	return s;
}

/*******************************************************************************
*	�E�B���h�E�ʒu(�����)
*******************************************************************************/
D3DXVECTOR2 Application::GetWindowPosition()
{
	RECT r;
	GetWindowRect(m_Window, &r);
	D3DXVECTOR2 p = { (float)(r.left) + (GetSystemMetrics(SM_CXDLGFRAME)),
					(float)(r.top) + (GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION)) };
	return p;
}

/*******************************************************************************
*	���̉�ʂ��A�N�e�B�u���ǂ���
*******************************************************************************/
bool Application::GetIsWindowActive()
{
	HWND hWnd = GetActiveWindow();
	return m_Window == hWnd;
}

/*******************************************************************************
*	�E�B���h�E�L���v�V�����擾
*******************************************************************************/
const char* Application::GetWindowName()
{
	return m_WINDOW_NAME;
}
