/*******************************************************************************
*
*	�^�C�g���F	�f�o�b�O��ʗp�ÓI�N���X
*	�t�@�C���F	debugConsole.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#include "debugConsole.h"

void DebugConsole::Init()
{
	// �R���\�\���E�B���h�E�쐬
	AllocConsole();

	// �������ݏ���
	m_Out = 0;
	(void)freopen_s(&m_Out, "CON", "w", stdout);

	// �n���h���擾
	m_Console = GetStdHandle(STD_OUTPUT_HANDLE);

	// �J�[�\�����W������
	m_CursorPos.X = 0;
	m_CursorPos.Y = 0;
}
void DebugConsole::Uninit()
{
	// �R���\�[�����
	fclose(m_Out);
	FreeConsole();
}
void DebugConsole::Update()
{
	// �J�[�\��������Ɉړ�
	//SetConsoleCursorPosition(m_Console, m_CursorPos);
}
#endif
