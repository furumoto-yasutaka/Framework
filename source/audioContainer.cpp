/*******************************************************************************
*
*	タイトル：	サウンド管理用静的クラス
*	ファイル：	audioContainer.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "audioContainer.h"
#include "scene.h"
#include "manager.h"

#include <algorithm>

void AudioContainer::Init()
{
	InitMaster();

	// デモ用アセット
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
	// すべてのサウンドを解放
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
*	マスタースピーカーの初期化
*******************************************************************************/
void AudioContainer::InitMaster()
{
	// COM初期化
	(void)CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio生成
	XAudio2Create(&m_Xaudio, 0);

	// マスタリングボイス生成
	m_Xaudio->CreateMasteringVoice(&m_MasteringVoice);
}

/*******************************************************************************
*	マスタースピーカーの終了処理
*******************************************************************************/
void AudioContainer::UninitMaster()
{
	m_MasteringVoice->DestroyVoice();
	m_Xaudio->Release();
	CoUninitialize();
}

/*******************************************************************************
*	サウンド情報取得(名称で検索)
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
		string str = string("存在しない名称のサウンドの取得が要求されました。\n") + name;
		MessageBox(Application::GetWindow(),
			TEXT(str.c_str()),
			TEXT("audioContainer:Error"),
			MB_OK | MB_ICONERROR);
		return NULL;
	}
}

/*******************************************************************************
*	サウンド名称(コンテナ上の名前)取得(インスタンス参照)
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
		string str = string("存在しないサウンドの名称の取得が要求されました。");
		MessageBox(Application::GetWindow(),
			TEXT(str.c_str()),
			TEXT("audioContainer:Error"),
			MB_OK | MB_ICONERROR);
		return "";
	}
}

/*******************************************************************************
*	サウンド追加
*******************************************************************************/
void AudioContainer::AddAudio(string name, string path, float volume)
{
	string fullPath = "asset\\audio\\" + path;

	// 要素がマップに存在するか確認
	if (m_AudioInfo.count(string(name)))
	{
		string str = string("すでに存在する名称でサウンドの追加が要求されました。\n") + fullPath + "\nこのサウンドは読み込まれません。";
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
*	サウンド取得(パスで検索)
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
