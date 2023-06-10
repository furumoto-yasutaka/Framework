/*******************************************************************************
*
*	�^�C�g���F	�f�o�b�O�E�B���h�E����N���X
*	�t�@�C���F	debugWindow.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "main.h"
#include "debugManager.h"

class DebugWindow
{
protected:
	string		m_WindowName;		// �E�B���h�E��
	bool		m_IsActive;			// �E�B���h�E���N�����Ă��邩
	ImVec2		m_WindowSize;		// �E�B���h�E�T�C�Y
	ImVec2		m_WindowPos;		// �E�B���h�E�T�C�Y
	D3DXVECTOR4 m_WindowClearColor;	// �E�B���h�E�̐F

public:
	DebugWindow(bool isActive = true)
	{
		m_IsActive = isActive;
		m_WindowPos = ImVec2(360.0f, 50.0f);
		m_WindowSize = ImVec2(200.0f, 300.0f);
		m_WindowClearColor = D3DXVECTOR4(0.45f, 0.55f, 0.60f, 1.00f);
	}
	virtual ~DebugWindow() {}

	virtual void Draw() = 0;

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	string GetName() { return m_WindowName; }

	bool GetIsActive() { return m_IsActive; }
	void SetIsActive(bool isActive) { m_IsActive = isActive; }
	/////////////////////////////
};
#endif
