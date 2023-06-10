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

const char* g_CLASS_NAME = "AppClass";			// �E�B���h�E�N���X��
const char* g_WINDOW_NAME = "Framework";	// �E�B���h�E�L���v�V����
HWND g_Window;									// �E�B���h�E�n���h��
bool g_IsFullScreen = false;					// �t���X�N���[���t���O
//bool g_IsSleep = false;							// �Q�[����~�t���O
//bool g_IsWireFrame = false;						// ���C���t���[���\���t���O
//bool g_IsMoveCursor = true;
#ifdef _DEBUG
int g_CountFPS;				// FPS�l
char g_DebugStr[2048];		// �E�B���h�E�L���v�V�����ɂȂ��ĕ\��������
#endif

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�����_���V�[�h�̏�����
	srand((unsigned int)time(NULL));

	// �E�B���h�E�N���X�쐬
	WNDCLASSEX wcex =
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
		g_CLASS_NAME,
		NULL
	};

	// �E�B���h�E�쐬�ׁ̈A�E�B���h�E�N���X��o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�쐬
	g_Window = CreateWindowEx(0,
		g_CLASS_NAME,
		g_WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		(int)(GetSystemMetrics(SM_CXSCREEN) * ((1.0f - WINDOW_RATE_WIDTH) * 0.5f)),		// �E�B���h�E�ʒu(X)
		(int)(GetSystemMetrics(SM_CYSCREEN) * ((1.0f - WINDOW_RATE_HEIGHT) * 0.33f)),	// �E�B���h�E�ʒu(Y)
		(int)(WINDOW_RESOLUTION_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),									// �E�B���h�E�T�C�Y(X)
		(int)(WINDOW_RESOLUTION_HEIGHT + GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),	// �E�B���h�E�T�C�Y(Y)
		NULL,
		NULL,
		hInstance,
		NULL);

	//ShowCursor(g_IsMoveCursor);

	// �Q�[����������
	Manager::Init(hInstance);

	// �E�B���h�E��\��
	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	// �^�C�}�[�̉𑜓x���w��(�~���b)
	timeBeginPeriod(1);

	// �v���p�ϐ�������
	DWORD dwExecLastTime = timeGetTime();
	DWORD dwCurrentTime = 0;
#ifdef _DEBUG
	strcpy(g_DebugStr, g_WINDOW_NAME);
	DWORD dwFPSLastTime = dwExecLastTime;
	int dwFrameCount = 0;
#endif

	MSG msg;
	while (1)
	{
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else //if (!g_IsSleep)
		{
			// �o�ߎ��Ԃ��X�V
			dwCurrentTime = timeGetTime();

			// 1/60�b���o�߂�����X�V�E�`������s����
			if ((dwCurrentTime - dwExecLastTime) >= (1000.0f / 60))
			{
				dwExecLastTime = dwCurrentTime;
#ifdef _DEBUG
				dwFrameCount++;
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
			if ((dwCurrentTime - dwFPSLastTime) >= 1000.0f)
			{
				g_CountFPS = dwFrameCount;
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
				wsprintf(g_DebugStr, g_WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(g_Window, g_DebugStr);
			}
#endif
		}
	}

	timeEndPeriod(1);

	UnregisterClass(g_CLASS_NAME, wcex.hInstance);

	Manager::Uninit();
	_CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}

#ifdef _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

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
			g_IsFullScreen = !g_IsFullScreen;
			Renderer::GetSwapChain()->SetFullscreenState(g_IsFullScreen, NULL);
			break;
		default: break;
		}
		break;
	default: break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*******************************************************************************
*	�E�B���h�E�擾
*******************************************************************************/
HWND GetWindow()
{
	return g_Window;
}

/*******************************************************************************
*	�E�B���h�E�T�C�Y�擾
*******************************************************************************/
D3DXVECTOR2 GetWindowSize()
{
	RECT r;
	GetWindowRect(g_Window, &r);
	D3DXVECTOR2 s = { (float)(r.right - r.left - (GetSystemMetrics(SM_CXDLGFRAME) * 2)),
				(float)(r.bottom - r.top - (GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION))) };
	return s;
}

/*******************************************************************************
*	�E�B���h�E�ʒu(�����)
*******************************************************************************/
D3DXVECTOR2 GetWindowPosition()
{
	RECT r;
	GetWindowRect(g_Window, &r);
	D3DXVECTOR2 p = { (float)(r.left) + (GetSystemMetrics(SM_CXDLGFRAME)),
					(float)(r.top) + (GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION)) };
	return p;
}

bool GetIsFullScreen()
{
	return g_IsFullScreen;
}

/*******************************************************************************
*	���̉�ʂ��A�N�e�B�u���ǂ���
*******************************************************************************/
bool GetIsWindowActive()
{
	HWND hWnd = GetActiveWindow();
	return g_Window == hWnd;
}

/*******************************************************************************
*	�E�B���h�E�L���v�V�����擾
*******************************************************************************/
const char* GetWindowName()
{
	return g_WINDOW_NAME;
}
