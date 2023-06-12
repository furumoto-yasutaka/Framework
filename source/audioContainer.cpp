/*******************************************************************************
*
*	�^�C�g���F	�T�E���h�Ǘ��p�ÓI�N���X
*	�t�@�C���F	audioContainer.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "audioContainer.h"
#include "scene.h"
#include "manager.h"

#include <algorithm>

void AudioContainer::Init()
{
	InitMaster();

	// �f���p�A�Z�b�g
	{
		// BGM
		AddAudio("TestBgm1", "Bgm\\TitleBgm.wav", 0.05f);
		AddAudio("TestBgm2", "Bgm\\GameBgm.wav", 0.01f);
		AddAudio("TestBgm3", "Bgm\\StageSelectBgm.wav", 0.05f);
		AddAudio("TestBgm4", "Bgm\\ResultBgm.wav", 0.05f);

		// SE
	}
}

void AudioContainer::Uninit()
{
	// ���ׂẴT�E���h�����
	auto itr = m_AudioInfo.begin();
	while (itr != m_AudioInfo.end())
	{
		itr->second->Unload();
		delete itr->second;
		itr = m_AudioInfo.erase(itr);
	}

	UninitMaster();
}

/*******************************************************************************
*	�}�X�^�[�X�s�[�J�[�̏�����
*******************************************************************************/
void AudioContainer::InitMaster()
{
	// COM������
	(void)CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio����
	XAudio2Create(&m_Xaudio, 0);

	// �}�X�^�����O�{�C�X����
	m_Xaudio->CreateMasteringVoice(&m_MasteringVoice);
}

/*******************************************************************************
*	�}�X�^�[�X�s�[�J�[�̏I������
*******************************************************************************/
void AudioContainer::UninitMaster()
{
	m_MasteringVoice->DestroyVoice();
	m_Xaudio->Release();
	CoUninitialize();
}

/*******************************************************************************
*	�T�E���h���擾(���̂Ō���)
*******************************************************************************/
AudioInfo* AudioContainer::GetAudio_InName(string name)
{
	auto itr = m_AudioInfo.find(name);

	if (itr != m_AudioInfo.end())
	{
		return itr->second;
	}
	else
	{
		string str = string("���݂��Ȃ����̂̃T�E���h�̎擾���v������܂����B\n") + name;
		MessageBox(Application::GetWindow(),
			TEXT(str.c_str()),
			TEXT("audioContainer:Error"),
			MB_OK | MB_ICONERROR);
		return NULL;
	}
}

/*******************************************************************************
*	�T�E���h����(�R���e�i��̖��O)�擾(�C���X�^���X�Q��)
*******************************************************************************/
string AudioContainer::GetName_InAudio(AudioInfo* info)
{
	auto itr = find_if(m_AudioInfo.begin(), m_AudioInfo.end(),
		[&](pair<string, AudioInfo*> pair) { return pair.second == info; });
	
	if (itr != m_AudioInfo.end())
	{
		return itr->first;
	}
	else
	{
		string str = string("���݂��Ȃ��T�E���h�̖��̂̎擾���v������܂����B");
		MessageBox(Application::GetWindow(),
			TEXT(str.c_str()),
			TEXT("audioContainer:Error"),
			MB_OK | MB_ICONERROR);
		return "";
	}
}

/*******************************************************************************
*	�T�E���h�ǉ�
*******************************************************************************/
void AudioContainer::AddAudio(string name, string path, float volume)
{
	string fullPath = "asset\\audio\\" + path;

	// �v�f���}�b�v�ɑ��݂��邩�m�F
	if (m_AudioInfo.count(string(name)))
	{
		string str = string("���łɑ��݂��閼�̂ŃT�E���h�̒ǉ����v������܂����B\n") + fullPath + "\n���̃T�E���h�͓ǂݍ��܂�܂���B";
		MessageBox(Application::GetWindow(),
			TEXT(str.c_str()),
			TEXT("audioContainer:Error"),
			MB_OK | MB_ICONERROR);
	}
	else
	{
		AudioInfo* audio = CreateAudio_InPath(fullPath, volume);
		audio->Load();
		m_AudioInfo[name] = audio;
	}
}

/*******************************************************************************
*	�T�E���h�擾(�p�X�Ō���)
*******************************************************************************/
AudioInfo* AudioContainer::CreateAudio_InPath(string path, float volume)
{
	auto itr = find_if(m_AudioInfo.begin(), m_AudioInfo.end(),
		[&](pair<string, AudioInfo*> pair) { return strcmp(pair.second->GetPath(), path.c_str()) == 0; });

	if (itr != m_AudioInfo.end())
	{
		return itr->second;
	}
	else
	{
		return DBG_NEW AudioInfo(path.c_str(), volume);
	}
}
