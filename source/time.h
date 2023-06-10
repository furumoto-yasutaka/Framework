/*******************************************************************************
*
*	�^�C�g���F�@���ԊǗ��p�ÓI�N���X
*	�t�@�C���F	time.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"

class Time
{
private:
	static inline float m_DeltaTimeSec;		// �t���[���Ԍo�ߎ���(�b)
	static inline DWORD m_DeltaTimeMsec;	// �t���[���Ԍo�ߎ���(�~���b)
	static inline float m_PlayTimeSec;		// �N�����猻�݂܂ł̎���(�b)
	static inline DWORD m_PlayTimeMsec;		// �N�����猻�݂܂ł̎���(�~���b)
	static inline float m_SystemTimeSec;	// �V�X�e��������(�b)
	static inline DWORD m_SystemTimeMsec;	// �V�X�e��������(�~���b)

private:
	Time() {}
public:
	static void Init();
	static void Uninit() {}
	static void Update();
	
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	static float GetDeltaTimeSec() { return m_DeltaTimeSec; }
	static DWORD GetDeltaTimeMsec() { return m_DeltaTimeMsec; }

	static float GetPlayTimeSec() { return m_PlayTimeSec; }
	static DWORD GetPlayTimeMsec() { return m_PlayTimeMsec; }

	static float GetSystemTimeSec() { return m_SystemTimeSec; }
	static DWORD GetSystemTimeMsec() { return m_SystemTimeMsec; }
	/////////////////////////////
};
