/*******************************************************************************
*
*	�^�C�g���F	�T�E���h�Ǘ��p�ÓI�N���X
*	�t�@�C���F	audioContainer.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "audio.h"

#include <unordered_map>
#include <list>

class AudioContainer
{
private:
	// ���v�f
	// ����1	string�^		�T�E���h����(�L�[)
	// ����2	AudioInfo*�^	�T�E���h���
	static inline unordered_map<string, AudioInfo*> m_AudioInfo;		// �T�E���h���X�g

	static inline IXAudio2*					m_Xaudio = NULL;			// XAudio2�̃C���^�[�t�F�[�X
	static inline IXAudio2MasteringVoice*	m_MasteringVoice = NULL;	// �}�X�^�[�X�s�[�J�[

private:
	AudioContainer() {}
public:
	static void Init();
	static void Uninit();

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	
	// �T�E���h���擾(���̂Ō���)
	static AudioInfo* GetAudio_InName(string name);

	// �T�E���h����(�R���e�i��̖��O)�擾(�C���X�^���X�Q��)
	static string GetName_InAudio(AudioInfo* info);

	static IXAudio2* GetXaudio() { return m_Xaudio; }
	/////////////////////////////
private:
	// �}�X�^�[�X�s�[�J�[�̏�����
	static void InitMaster();

	// �}�X�^�[�X�s�[�J�[�̏I������
	static void UninitMaster();

	// �T�E���h�ǉ�
	static void AddAudio(string name, string path, float volume);

	// �T�E���h�擾(�p�X�Ō���)�@���Ȃ���ΐV��������
	static AudioInfo* CreateAudio_InPath(string path, float volume);
};
