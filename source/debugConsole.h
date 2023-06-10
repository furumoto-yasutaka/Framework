/*******************************************************************************
*
*	�^�C�g���F	�f�o�b�O��ʗp�ÓI�N���X
*	�t�@�C���F	debugConsole.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "main.h"

class DebugConsole
{
private:
	static inline FILE*		m_Out;
	static inline HANDLE	m_Console;		// �R���\�[���n���h��
	static inline COORD		m_CursorPos;	// �J�[�\�����W

private:
	DebugConsole() {}
public:
	static void Init();
	static void Uninit();
	static void Update();
};
#endif
