/*******************************************************************************
*
*	�^�C�g���F	���C��
*	�t�@�C���F	main.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#define NOMINMAX

#pragma warning(push)
#pragma warning(disable:4005)

#include "dinput.h"
#include "mmsystem.h"
#include "math.h"

#include <dwrite.h>
#include <D2D1.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <d3d11.h>
#include <d3dx9.h>
#include <d3dcompiler.h>
#include <d3dx11.h>

#pragma warning(pop)

#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "bcrypt.lib")

using namespace std;

//---------------------------------------
// CRT�ɂ�郁�������[�N�`�F�b�N�p
//---------------------------------------
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

//---------------------------------------
// �}�N����`
//---------------------------------------
#define SYSTEM_RESOLUTION_WIDTH		(float)GetSystemMetrics(SM_CXSCREEN)	// ��ʉ𑜓x(X)
#define SYSTEM_RESOLUTION_HEIGHT	(float)GetSystemMetrics(SM_CYSCREEN)	// ��ʉ𑜓x(Y)

#define WINDOW_RATE_WIDTH	(0.8f)	// �E�B���h�E���[�h���̃X�N���[���T�C�Y�{��(X)(�����l)
#define WINDOW_RATE_HEIGHT	(0.8f)	// �E�B���h�E���[�h���̃X�N���[���T�C�Y�{��(Y)(�����l)
#define WINDOW_RESOLUTION_WIDTH		(SYSTEM_RESOLUTION_WIDTH * WINDOW_RATE_WIDTH)	// ��ʉ𑜓x(X)
#define WINDOW_RESOLUTION_HEIGHT	(SYSTEM_RESOLUTION_HEIGHT * WINDOW_RATE_HEIGHT)// ��ʉ𑜓x(Y)

// �E�B���h�E�擾
HWND GetWindow();

// �E�B���h�E�T�C�Y�擾
D3DXVECTOR2 GetWindowSize();

// �E�B���h�E�ʒu(�����)
D3DXVECTOR2 GetWindowPosition();

// �t���X�N���[�����ǂ���
bool GetIsFullScreen();

// ���̉�ʂ��A�N�e�B�u���ǂ���
bool GetIsWindowActive();

// �E�B���h�E�L���v�V�����擾
const char* GetWindowName();
