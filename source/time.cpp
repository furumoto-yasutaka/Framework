/*******************************************************************************
*
*	�^�C�g���F�@���ԊǗ��p�ÓI�N���X
*	�t�@�C���F	time.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "time.h"

void Time::Init()
{
	// ������
	m_DeltaTimeMsec = 0;
	m_PlayTimeMsec = 0;
	m_SystemTimeMsec = timeGetTime();
}

void Time::Update()
{
	// �e�ϐ����X�V
	DWORD prevTime = m_SystemTimeMsec;
	m_SystemTimeMsec = timeGetTime();
	m_DeltaTimeMsec = m_SystemTimeMsec - prevTime;
	m_PlayTimeMsec += m_DeltaTimeMsec;

	// �~���b����b�ɕϊ�
	m_DeltaTimeSec = m_DeltaTimeMsec * 0.001f;
	m_PlayTimeSec = m_PlayTimeMsec * 0.001f;
	m_SystemTimeSec = m_SystemTimeMsec * 0.001f;
}
