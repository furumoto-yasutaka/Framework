/*******************************************************************************
*
*	�^�C�g���F	�T�E���h�Đ��p�R���|�[�l���g
*	�t�@�C���F	audioSpeaker.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "audioSpeaker.h"
#include "audioContainer.h"
#include "uniqueNamingSystem.h"

void AudioSpeaker::Init()
{
	m_SpeakerList.push_back(this);
}

void AudioSpeaker::Uninit()
{
	for (auto itr = m_PlayInfo.begin(); itr != m_PlayInfo.end(); itr++)
	{
		PlayInfo* info = itr->second;

		// �T�E���h���~�����
		if (info->SourceVoice)
		{
			info->SourceVoice->Stop();
			info->SourceVoice->DestroyVoice();
		}
		// �X�g���[�~���O�p�̗v�f�����
		if (info->AudioInfo->GetIsStreaming())
		{
			mmioClose(*info->Hmmio, 0);
			delete info->Hmmio;
			delete info->CallBack;
			delete[] info->PrimaryBuffer;
			delete[] info->SecondaryBuffer;
		}

		delete info;
	}
	
	m_SpeakerList.remove(this);
}

/*******************************************************************************
*	�Đ��J�n(�߂�l�͑���p�̎��ʃL�[)
*******************************************************************************/
string AudioSpeaker::Play(string name, bool isLoop, float volume)
{
	string Key = UniqueNamingSystem::CreateUniqueName<PlayInfo*>(name, m_PlayInfo);
	PlayInfo* info = SetAudio(Key, name, isLoop, volume);

	if (info->AudioInfo->GetIsStreaming())
	{// �X�g���[�~���O�Đ�
		InitBuffer(info);
	}
	else
	{// �ʏ�Đ�
		// �I�[�f�B�I�o�b�t�@�ݒ�
		XAUDIO2_BUFFER bufinfo;
		memset(&bufinfo, 0x00, sizeof(bufinfo));
		bufinfo.AudioBytes = info->AudioInfo->GetAudioBytes();
		bufinfo.pAudioData = info->AudioInfo->GetAudioData();

		// ���[�v�ݒ�
		if (isLoop)
		{
			bufinfo.LoopBegin = 0;
			bufinfo.LoopLength = info->AudioInfo->GetAudioBytes() / info->AudioInfo->GetWaveFormat()->nBlockAlign;
			bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		// �o�b�t�@�𔽉f
		info->SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
	}

	// �Đ��J�n
	info->SourceVoice->SetVolume(info->Volume);
	info->SourceVoice->Start();
	info->State = AudioInfo::AudioState::Play;

	return Key;
}

/*******************************************************************************
*	�Đ���~(�L�[�Q��)
*******************************************************************************/
void AudioSpeaker::Stop_FromKey(string key)
{
	auto itr = m_PlayInfo.find(key);
	// �L�[���L�����m�F
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;

	// �T�E���h���~�����
	info->SourceVoice->Stop();
	info->SourceVoice->DestroyVoice();
	// �X�g���[�~���O�p�̗v�f�����
	if (info->AudioInfo->GetIsStreaming())
	{
		mmioClose(*info->Hmmio, 0);
		delete info->CallBack;
		delete info->PrimaryBuffer;
		delete info->SecondaryBuffer;
	}
	delete info;
	
	// �Đ���񃊃X�g����폜
	m_PlayInfo.erase(key);
}

/*******************************************************************************
*	�Đ���~(���̎Q�� ����������ꍇ�͌��ݍĐ����̒��ōł��A�Ԃ��������T�E���h)
*******************************************************************************/
void AudioSpeaker::Stop_FromAudioName(string name)
{
	auto itr = GetIterator_FromAudioName(name);
	// �����T�E���h���̂̍Đ���񂪑��݂��邩�m�F
	if (itr == m_PlayInfo.end())
	{
		AudioNameUnknownError(name);
		return;
	}

	PlayInfo* info = itr->second;

	// �T�E���h���~�����
	info->SourceVoice->Stop();
	info->SourceVoice->DestroyVoice();
	// �X�g���[�~���O�p�̗v�f�����
	if (info->AudioInfo->GetIsStreaming())
	{
		mmioClose(*info->Hmmio, 0);
		delete info->CallBack;
		delete info->PrimaryBuffer;
		delete info->SecondaryBuffer;
	}
	delete info;

	// �Đ���񃊃X�g����폜
	m_PlayInfo.erase(itr);
}

/*******************************************************************************
*	�Đ��ĊJ
*******************************************************************************/
void AudioSpeaker::Resume(string key)
{
	auto itr = m_PlayInfo.find(key);
	// �L�[���L�����m�F
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;
	// �Đ���~���łȂ��ꍇ�͏I��
	if (info->State != AudioInfo::AudioState::Pause) { return; }

	// �Đ��J�n
	info->SourceVoice->SetVolume(info->Volume);
	info->SourceVoice->Start();
	info->State = AudioInfo::AudioState::Play;
}

/*******************************************************************************
*	�ꎞ��~
*******************************************************************************/
void AudioSpeaker::Pause(string key)
{
	auto itr = m_PlayInfo.find(key);
	// �L�[���L�����m�F
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;
	// �Đ����łȂ��ꍇ�͏I��
	if (info->State != AudioInfo::AudioState::Play) { return; }

	// �Đ���~
	info->SourceVoice->Stop();
	info->State = AudioInfo::AudioState::Pause;
}

/*******************************************************************************
*	���ʐݒ�
*******************************************************************************/
void AudioSpeaker::SetVolume(string key, float volume)
{
	auto itr = m_PlayInfo.find(key);
	// �L�[���L�����m�F
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;

	// �Đ��J�n
	info->Volume = volume;
	info->SourceVoice->SetVolume(info->Volume);
}

/*******************************************************************************
*	�ǉ��̃o�b�t�@��ǂݍ���
*******************************************************************************/
void AudioSpeaker::AddNextBuffer(string key)
{
	auto itr = m_PlayInfo.find(key);
	// �L�[���L�����m�F
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;
	bool endOfSteramFlag = CheckSeek(info);
	// ���[�v�����̐ݒ�Ńf�[�^�̏I�[�ɒB���Ă���ꍇ��
	// �ǉ��̕K�v�͂Ȃ��׏I������
	if (endOfSteramFlag && !info->IsLoop) { return; }

	//------------------------
	// �f�[�^�o�b�t�@����ւ�
	//------------------------
	BYTE* temp = info->PrimaryBuffer;
	LONG primaryBufLength;

	info->PrimaryBuffer = info->SecondaryBuffer;
	info->SecondaryBuffer = temp;
	primaryBufLength = info->SecondaryBufLength;

	//------------------------
	// �f�[�^�o�b�t�@�ǂݍ���
	//------------------------
	info->SecondaryBufLength = mmioRead(*info->Hmmio, (HPSTR)info->SecondaryBuffer, CalcBufferLength(info));
	info->SeekCursor += info->SecondaryBufLength;

	//------------------------
	// �I�[�f�B�I�o�b�t�@�ݒ�
	//------------------------
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = primaryBufLength;
	bufinfo.pAudioData = info->PrimaryBuffer;
	if (endOfSteramFlag)
	{
		// �f�[�^�̏I�[�̏ꍇ�͏I���t���O��n��
		bufinfo.Flags = XAUDIO2_END_OF_STREAM;
	}

	// �o�b�t�@�𔽉f
	info->SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
}

/*******************************************************************************
*	�T�E���h��o�^
*******************************************************************************/
AudioSpeaker::PlayInfo* AudioSpeaker::SetAudio(string key, string name, bool isLoop, float volume)
{
	PlayInfo* info = DBG_NEW PlayInfo();

	info->AudioName = name;
	info->AudioInfo = AudioContainer::GetAudio_InName(name);
	info->State = AudioInfo::AudioState::Play;
	info->IsLoop = isLoop;
	info->Volume = info->AudioInfo->GetVolume() * volume;

	// �X�g���[�~���O�Đ��p
	if (info->AudioInfo->GetIsStreaming())
	{
		info->Hmmio = DBG_NEW HMMIO();
		info->AudioInfo->Open(info->Hmmio);
		info->CallBack = DBG_NEW AudioCallback(this, key);
		info->PrimaryBuffer = DBG_NEW unsigned char[GetDefaultBufferLength(info)];
		info->SecondaryBuffer = DBG_NEW unsigned char[GetDefaultBufferLength(info)];
		info->SecondaryBufLength = 0;
		info->SeekCursor = 0;
	}

	// �T�E���h�\�[�X����
	AudioContainer::GetXaudio()->CreateSourceVoice(
		&info->SourceVoice,
		info->AudioInfo->GetWaveFormat(),
		0,
		XAUDIO2_DEFAULT_FREQ_RATIO,
		info->AudioInfo->GetIsStreaming() ? info->CallBack : NULL);
	assert(info->SourceVoice);

	// �Đ��������X�g�ɒǉ�
	m_PlayInfo[key] = info;

	return info;
}

/*******************************************************************************
*	�Đ����̃C�e���[�^���擾
* 	(���̎Q�� ����������ꍇ�͌��ݍĐ����̒��ōł��A�Ԃ��������T�E���h)
*******************************************************************************/
map<string, AudioSpeaker::PlayInfo*>::iterator AudioSpeaker::GetIterator_FromAudioName(string name)
{
	auto itr = m_PlayInfo.begin();

	for (; itr != m_PlayInfo.end() && itr->second->AudioName != name; itr++);

	return itr;
}

/*******************************************************************************
*	�L���łȂ��L�[���n���ꂽ�ۂ̃G���[����
*******************************************************************************/
void AudioSpeaker::KeyUnknownError(string key)
{
	string str = string("���݂��Ȃ��L�[�ł̎Q�Ƃ��v������܂����B\n") + key;
	MessageBox(GetWindow(),
		TEXT(str.c_str()),
		TEXT("audioSpeaker:Error"),
		MB_OK | MB_ICONERROR);
}

/*******************************************************************************
*	�Đ���񃊃X�g�ɑ��݂��Ȃ��T�E���h���̂��n���ꂽ�ۂ̃G���[����
*******************************************************************************/
void AudioSpeaker::AudioNameUnknownError(string name)
{
	string str = string("�Đ���񃊃X�g�ɑ��݂��Ȃ��T�E���h���̂ł̎Q�Ƃ��v������܂����B\n") + name;
	MessageBox(GetWindow(),
		TEXT(str.c_str()),
		TEXT("audioSpeaker:Error"),
		MB_OK | MB_ICONERROR);
}

/*******************************************************************************
*	�o�b�t�@�T�C�Y�̕W���l���擾
*******************************************************************************/
LONG AudioSpeaker::GetDefaultBufferLength(PlayInfo* info)
{
	// 1�b������̃o�C�g���@���@�o�b�t�@������̕b��
	return info->AudioInfo->GetWaveFormat()->nAvgBytesPerSec * m_SECOND_PER_BUFFER;
}

/*******************************************************************************
*	�X�g���[�~���O�p�o�b�t�@��������
*******************************************************************************/
void AudioSpeaker::InitBuffer(PlayInfo* info)
{
	//------------------------
	// �f�[�^�o�b�t�@�ǂݍ���
	//------------------------
	LONG primaryBufLength;
	bool endOfSteramFlag = false;

	primaryBufLength = mmioRead(*info->Hmmio, (HPSTR)info->PrimaryBuffer, CalcBufferLength(info));
	info->SeekCursor += primaryBufLength;

	endOfSteramFlag = CheckSeek(info);
	if (!endOfSteramFlag || info->IsLoop)
	{
		info->SecondaryBufLength = mmioRead(*info->Hmmio, (HPSTR)info->SecondaryBuffer, CalcBufferLength(info));
		info->SeekCursor += info->SecondaryBufLength;
	}

	//------------------------
	// �I�[�f�B�I�o�b�t�@�ݒ�
	//------------------------
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = primaryBufLength;
	bufinfo.pAudioData = info->PrimaryBuffer;
	if (endOfSteramFlag)
	{
		// �f�[�^�̏I�[�̏ꍇ�͏I���t���O��n��
		bufinfo.Flags = XAUDIO2_END_OF_STREAM;
	}

	// �o�b�t�@�𔽉f
	info->SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
}

/*******************************************************************************
*	�m�ۉ\�ȃo�b�t�@�T�C�Y���v�Z
*******************************************************************************/
LONG AudioSpeaker::CalcBufferLength(PlayInfo* info)
{
	LONG defaultLen = GetDefaultBufferLength(info);
	LONG audioBytes = info->AudioInfo->GetAudioBytes();

	// �c��̃T�E���h�f�[�^�ʂ��o�b�t�@�T�C�Y�̕����傫�����ǂ���
	if (audioBytes - info->SeekCursor < defaultLen)
	{// �傫���ꍇ
		// �c��̃T�E���h�f�[�^�ʂ�Ԃ�
		return audioBytes - info->SeekCursor;
	}
	else
	{// �傫���Ȃ��ꍇ
		// �o�b�t�@�T�C�Y��Ԃ�
		return defaultLen;
	}
}

/*******************************************************************************
*	�V�[�N�ʒu���I�[�ɒB���Ă��邩�m�F����
*******************************************************************************/
bool AudioSpeaker::CheckSeek(PlayInfo* info)
{
	LONG audioBytes = info->AudioInfo->GetAudioBytes();

	// �f�[�^�̏I�[�܂œǂݍ��݂��I�����Ă��邩�ǂ���
	if (info->SeekCursor >= audioBytes)
	{
		if (info->IsLoop)
		{
			// �Ō�܂œǂݍ��ݍς݂̏ꍇ�̓V�[�N��擪�ɖ߂����[�v�ɔ�����
			mmioSeek(*info->Hmmio, -info->SeekCursor, SEEK_CUR);
			info->SeekCursor = 0;
		}
		return true;
	}
	else
	{
		return false;
	}
}

#ifdef _DEBUG
/*******************************************************************************
*	�C���X�y�N�^�[�\������
*******************************************************************************/
void AudioSpeaker::DrawInspector()
{
	if (ImGui::CollapsingHeader("AudioSpeaker"))
	{
		int i = 1;

		for (auto itr = m_PlayInfo.begin(); itr != m_PlayInfo.end(); itr++, i++)
		{
			string tempStr = to_string(i) + " : " + itr->second->AudioName;
			float tempFloat;

			if (ImGui::TreeNode(tempStr.c_str()))
			{
				switch (itr->second->State)
				{
					using enum AudioInfo::AudioState;
				case Play:
					tempStr = "Play";
					break;
				case Pause:
					tempStr = "Pause";
					break;
				}
				ImGui::Text("AudioState : %s", tempStr.c_str());

				ImGui::Text("IsLoop : %s", (itr->second->IsLoop ? "true" : "false"));

				tempFloat = itr->second->Volume;
				if (ImGui::SliderFloat("Volume", &tempFloat, 0.0f, 2.0f))
				{
					SetVolume(itr->first, tempFloat);
				}

				ImGui::TreePop();
			}
		}
	}
}
#endif


//�@�����@�R�[���o�b�N�N���X�̃��\�b�h�@����

/*******************************************************************************
*	XAUDIO2_END_OF_STREAM�t���O�𗧂Ă���Ԃ�
* 	���݂̃o�b�t�@�̍Ō�̃o�C�g������ꂽ����ɌĂяo�����
*******************************************************************************/
void AudioCallback::OnStreamEnd()
{
	// ���[�v�ݒ�ł͂Ȃ��ꍇ��~����
	if (!m_AudioSpeaker->GetPlayInfo_FromKey(m_Key)->IsLoop)
	{
		m_AudioSpeaker->Stop_FromKey(m_Key);
	}
}

/*******************************************************************************
*	���݂̃o�b�t�@�̍Ō�̃o�C�g������ꂽ����ɌĂяo�����
*******************************************************************************/
void AudioCallback::OnBufferEnd(void* bufferContext)
{
	m_AudioSpeaker->AddNextBuffer(m_Key);
}
