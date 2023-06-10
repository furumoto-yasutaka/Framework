/*******************************************************************************
*
*	�^�C�g���F	�T�E���h�Đ��p�R���|�[�l���g
*	�t�@�C���F	audioSpeaker.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"
#include "audio.h"

#include <list>
#include <string>
#include <map>

class AudioCallback;

class AudioSpeaker : public NotDrawComponent
{
public:
	// �Đ��p�̏����܂Ƃ߂��\����
	struct PlayInfo
	{
		string					AudioName;			// �T�E���h�̖���(�R���e�i��̖��O)
		AudioInfo*				AudioInfo;			// �T�E���h�̏ڍ׏��
		AudioInfo::AudioState	State;				// �T�E���h�̏��
		bool					IsLoop;				// ���[�v�̗L��
		float					Volume;				// ����
		IXAudio2SourceVoice*	SourceVoice;		// �X�s�[�J�[

		// �X�g���[�~���O�Đ����̂ݎg�p
		HMMIO*					Hmmio;				// �ǂݍ��ݗp�n���h��
		AudioCallback*			CallBack;			// �R�[���o�b�N�N���X
		BYTE*					PrimaryBuffer;		// �o�b�t�@1
		BYTE*					SecondaryBuffer;	// �o�b�t�@2
		LONG					SecondaryBufLength;	// �o�b�t�@2�̂����g�p���Ă���̈�
		LONG					SeekCursor;			// �t�@�C���̓ǂݍ��݈ʒu
	};

private:
	static inline list<AudioSpeaker*>	m_SpeakerList;				// �T�E���h�̑S�̑���p�C���X�^���X���X�g
	static inline const unsigned int	m_SECOND_PER_BUFFER = 1;	// �X�g���[�~���O�Đ��p�̃o�b�t�@�T�C�Y���T�E���h�̉��b�m�ۂ��邩

	// ���v�f
	// ����1	string�^	���ʃL�[(�T�E���h����(�A��))
	// ����2	PlayInfo*�^	�Đ����
	map<string, PlayInfo*>	m_PlayInfo;		// �Đ���񃊃X�g

public:
	AudioSpeaker(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~AudioSpeaker() {}

	void Init();
	void Uninit() override;
	void Update() override {}

	// �Đ��J�n(�߂�l�͑���p�̎��ʃL�[)
	string Play(string name, bool isLoop = false, float volume = 1.0f);

	// �Đ���~(�L�[�Q��)
	void Stop_FromKey(string key);

	// �Đ���~(���̎Q�� ����������ꍇ�͌��ݍĐ����̒��ōł��A�Ԃ��������T�E���h)
	void Stop_FromAudioName(string name);

	// �Đ��ĊJ
	void Resume(string key);

	// �ꎞ��~
	void Pause(string key);

	// ���ʐݒ�
	void SetVolume(string key, float volume);

	/////////////////////////////////////////////////
	//�@�����@�X�g���[�~���O�Đ��p���\�b�h�@�����@//

	// �ǉ��̃o�b�t�@��ǂݍ���
	void AddNextBuffer(string key);

	// �Đ������擾(�L�[�Q��)
	PlayInfo* GetPlayInfo_FromKey(string key) { return m_PlayInfo[key]; }

	/////////////////////////////////////////////////
private:
	// �T�E���h��o�^
	PlayInfo* SetAudio(string key, string name, bool isLoop, float volume);

	// �Đ����̃C�e���[�^���擾
	// (���̎Q�� ����������ꍇ�͌��ݍĐ����̒��ōł��A�Ԃ��������T�E���h)
	map<string, PlayInfo*>::iterator GetIterator_FromAudioName(string name);
	
	// �L���łȂ��L�[���n���ꂽ�ۂ̃G���[����
	void KeyUnknownError(string key);

	// �Đ���񃊃X�g�ɑ��݂��Ȃ��T�E���h���̂��n���ꂽ�ۂ̃G���[����
	void AudioNameUnknownError(string name);

	/////////////////////////////////////////////////
	//�@�����@�X�g���[�~���O�Đ��p���\�b�h�@�����@//
	
	// �o�b�t�@�T�C�Y�̕W���l���擾
	LONG GetDefaultBufferLength(PlayInfo* info);

	// �X�g���[�~���O�p�o�b�t�@��������
	void InitBuffer(PlayInfo* info);

	// �m�ۉ\�ȃo�b�t�@�T�C�Y���v�Z
	LONG CalcBufferLength(PlayInfo* info);

	// �V�[�N�ʒu���I�[�ɒB���Ă��邩�m�F����
	bool CheckSeek(PlayInfo* info);
	/////////////////////////////////////////////////

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	map<string, PlayInfo*> GetPlayInfo() { return m_PlayInfo; }
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};

//--------------------------------------------
// �X�g���[�~���O�Đ��p�R�[���o�b�N�N���X
//--------------------------------------------
class AudioCallback : public IXAudio2VoiceCallback
{
private:
	AudioSpeaker*	m_AudioSpeaker;	// �X�s�[�J�[
	string			m_Key;			// �X�s�[�J�[�ɓo�^���ꂽ�T�E���h�̎��ʃL�[

public:
	AudioCallback(AudioSpeaker* audioSpeaker, string key)
		: m_AudioSpeaker(audioSpeaker), m_Key(key) {}
	~AudioCallback() {}

	// ????
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) {}
	// ????
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) {}
	// XAUDIO2_END_OF_STREAM�t���O�𗧂Ă���Ԃ�
	// ���݂̃o�b�t�@�̍Ō�̃o�C�g������ꂽ����ɌĂяo�����
	void STDMETHODCALLTYPE OnStreamEnd(void);
	// ���݂̃o�b�t�@�̍ŏ��̃o�C�g�������钼�O�ɌĂяo�����
	void STDMETHODCALLTYPE OnBufferStart(void* bufferContext) {}
	// ���݂̃o�b�t�@�̍Ō�̃o�C�g������ꂽ����ɌĂяo�����
	void STDMETHODCALLTYPE OnBufferEnd(void* bufferContext);
	// ���[�v�̏I���n�_�ɒB�����Ƃ��ɌĂяo�����
	void STDMETHODCALLTYPE OnLoopEnd(void* bufferContext) {}
	// �T�E���h�ɉ��炩�̃G���[�������������ɌĂяo�����(�H)
	void STDMETHODCALLTYPE OnVoiceError(void* bufferContext, HRESULT error) {}
};
