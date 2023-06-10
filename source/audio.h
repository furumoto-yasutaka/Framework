/*******************************************************************************
*
*	�^�C�g���F	�T�E���h�N���X
*	�t�@�C���F	audio.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include <xaudio2.h>
#include <vector>

class AudioInfo
{
public:
	// �T�E���h�̏��
	enum class AudioState
	{
		Play = 0,	// �Đ���
		Pause,		// �Đ���~
	};

private:
	char					m_Path[256];			// �t�@�C���p�X
	WAVEFORMATEX			m_WaveFormat;			// �t�@�C���\�����
	BYTE*					m_AudioData = NULL;		// �t�@�C���f�[�^(�X�g���[�~���O�Đ����͂Ȃ�)
	int						m_AudioBytes = 0;		// �o�C�g��
	bool					m_IsStreaming = false;	// �X�g���[�~���O�Đ��̗L��
	float					m_Volume = 1.0f;		// �T�E���h�̃{�����[��(0.0 �� �l)

public:
	AudioInfo(const char* path, float volume)
	{
		strcpy_s(m_Path, path);
		m_Volume = volume;
	}
	~AudioInfo() {}

	// ���[�h
	void Load();

	// �A�����[�h
	void Unload();

	// �t�@�C�����J��(�X�g���[�~���O�Đ��p)
	void Open(HMMIO* hmmio);

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	char* GetPath() { return m_Path; }

	WAVEFORMATEX* GetWaveFormat() { return &m_WaveFormat; }

	BYTE* GetAudioData() { return m_AudioData; }

	int GetAudioBytes() { return m_AudioBytes; }

	bool GetIsStreaming() { return m_IsStreaming; }

	float GetVolume() { return m_Volume; }
	/////////////////////////////
};
