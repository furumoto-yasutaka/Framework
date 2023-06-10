/*******************************************************************************
*
*	タイトル：	サウンド再生用コンポーネント
*	ファイル：	audioSpeaker.cpp
*	作成者：	古本 泰隆
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

		// サウンドを停止し解放
		if (info->SourceVoice)
		{
			info->SourceVoice->Stop();
			info->SourceVoice->DestroyVoice();
		}
		// ストリーミング用の要素を解放
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
*	再生開始(戻り値は操作用の識別キー)
*******************************************************************************/
string AudioSpeaker::Play(string name, bool isLoop, float volume)
{
	string Key = UniqueNamingSystem::CreateUniqueName<PlayInfo*>(name, m_PlayInfo);
	PlayInfo* info = SetAudio(Key, name, isLoop, volume);

	if (info->AudioInfo->GetIsStreaming())
	{// ストリーミング再生
		InitBuffer(info);
	}
	else
	{// 通常再生
		// オーディオバッファ設定
		XAUDIO2_BUFFER bufinfo;
		memset(&bufinfo, 0x00, sizeof(bufinfo));
		bufinfo.AudioBytes = info->AudioInfo->GetAudioBytes();
		bufinfo.pAudioData = info->AudioInfo->GetAudioData();

		// ループ設定
		if (isLoop)
		{
			bufinfo.LoopBegin = 0;
			bufinfo.LoopLength = info->AudioInfo->GetAudioBytes() / info->AudioInfo->GetWaveFormat()->nBlockAlign;
			bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		// バッファを反映
		info->SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
	}

	// 再生開始
	info->SourceVoice->SetVolume(info->Volume);
	info->SourceVoice->Start();
	info->State = AudioInfo::AudioState::Play;

	return Key;
}

/*******************************************************************************
*	再生停止(キー参照)
*******************************************************************************/
void AudioSpeaker::Stop_FromKey(string key)
{
	auto itr = m_PlayInfo.find(key);
	// キーが有効か確認
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;

	// サウンドを停止し解放
	info->SourceVoice->Stop();
	info->SourceVoice->DestroyVoice();
	// ストリーミング用の要素を解放
	if (info->AudioInfo->GetIsStreaming())
	{
		mmioClose(*info->Hmmio, 0);
		delete info->CallBack;
		delete info->PrimaryBuffer;
		delete info->SecondaryBuffer;
	}
	delete info;
	
	// 再生情報リストから削除
	m_PlayInfo.erase(key);
}

/*******************************************************************************
*	再生停止(名称参照 ※複数ある場合は現在再生中の中で最も連番が小さいサウンド)
*******************************************************************************/
void AudioSpeaker::Stop_FromAudioName(string name)
{
	auto itr = GetIterator_FromAudioName(name);
	// 同じサウンド名称の再生情報が存在するか確認
	if (itr == m_PlayInfo.end())
	{
		AudioNameUnknownError(name);
		return;
	}

	PlayInfo* info = itr->second;

	// サウンドを停止し解放
	info->SourceVoice->Stop();
	info->SourceVoice->DestroyVoice();
	// ストリーミング用の要素を解放
	if (info->AudioInfo->GetIsStreaming())
	{
		mmioClose(*info->Hmmio, 0);
		delete info->CallBack;
		delete info->PrimaryBuffer;
		delete info->SecondaryBuffer;
	}
	delete info;

	// 再生情報リストから削除
	m_PlayInfo.erase(itr);
}

/*******************************************************************************
*	再生再開
*******************************************************************************/
void AudioSpeaker::Resume(string key)
{
	auto itr = m_PlayInfo.find(key);
	// キーが有効か確認
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;
	// 再生停止中でない場合は終了
	if (info->State != AudioInfo::AudioState::Pause) { return; }

	// 再生開始
	info->SourceVoice->SetVolume(info->Volume);
	info->SourceVoice->Start();
	info->State = AudioInfo::AudioState::Play;
}

/*******************************************************************************
*	一時停止
*******************************************************************************/
void AudioSpeaker::Pause(string key)
{
	auto itr = m_PlayInfo.find(key);
	// キーが有効か確認
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;
	// 再生中でない場合は終了
	if (info->State != AudioInfo::AudioState::Play) { return; }

	// 再生停止
	info->SourceVoice->Stop();
	info->State = AudioInfo::AudioState::Pause;
}

/*******************************************************************************
*	音量設定
*******************************************************************************/
void AudioSpeaker::SetVolume(string key, float volume)
{
	auto itr = m_PlayInfo.find(key);
	// キーが有効か確認
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;

	// 再生開始
	info->Volume = volume;
	info->SourceVoice->SetVolume(info->Volume);
}

/*******************************************************************************
*	追加のバッファを読み込む
*******************************************************************************/
void AudioSpeaker::AddNextBuffer(string key)
{
	auto itr = m_PlayInfo.find(key);
	// キーが有効か確認
	if (itr == m_PlayInfo.end())
	{
		KeyUnknownError(key);
		return;
	}

	PlayInfo* info = itr->second;
	bool endOfSteramFlag = CheckSeek(info);
	// ループ無しの設定でデータの終端に達している場合は
	// 追加の必要はない為終了する
	if (endOfSteramFlag && !info->IsLoop) { return; }

	//------------------------
	// データバッファ入れ替え
	//------------------------
	BYTE* temp = info->PrimaryBuffer;
	LONG primaryBufLength;

	info->PrimaryBuffer = info->SecondaryBuffer;
	info->SecondaryBuffer = temp;
	primaryBufLength = info->SecondaryBufLength;

	//------------------------
	// データバッファ読み込み
	//------------------------
	info->SecondaryBufLength = mmioRead(*info->Hmmio, (HPSTR)info->SecondaryBuffer, CalcBufferLength(info));
	info->SeekCursor += info->SecondaryBufLength;

	//------------------------
	// オーディオバッファ設定
	//------------------------
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = primaryBufLength;
	bufinfo.pAudioData = info->PrimaryBuffer;
	if (endOfSteramFlag)
	{
		// データの終端の場合は終了フラグを渡す
		bufinfo.Flags = XAUDIO2_END_OF_STREAM;
	}

	// バッファを反映
	info->SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
}

/*******************************************************************************
*	サウンドを登録
*******************************************************************************/
AudioSpeaker::PlayInfo* AudioSpeaker::SetAudio(string key, string name, bool isLoop, float volume)
{
	PlayInfo* info = DBG_NEW PlayInfo();

	info->AudioName = name;
	info->AudioInfo = AudioContainer::GetAudio_InName(name);
	info->State = AudioInfo::AudioState::Play;
	info->IsLoop = isLoop;
	info->Volume = info->AudioInfo->GetVolume() * volume;

	// ストリーミング再生用
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

	// サウンドソース生成
	AudioContainer::GetXaudio()->CreateSourceVoice(
		&info->SourceVoice,
		info->AudioInfo->GetWaveFormat(),
		0,
		XAUDIO2_DEFAULT_FREQ_RATIO,
		info->AudioInfo->GetIsStreaming() ? info->CallBack : NULL);
	assert(info->SourceVoice);

	// 再生情報をリストに追加
	m_PlayInfo[key] = info;

	return info;
}

/*******************************************************************************
*	再生情報のイテレータを取得
* 	(名称参照 ※複数ある場合は現在再生中の中で最も連番が小さいサウンド)
*******************************************************************************/
map<string, AudioSpeaker::PlayInfo*>::iterator AudioSpeaker::GetIterator_FromAudioName(string name)
{
	auto itr = m_PlayInfo.begin();

	for (; itr != m_PlayInfo.end() && itr->second->AudioName != name; itr++);

	return itr;
}

/*******************************************************************************
*	有効でないキーが渡された際のエラー処理
*******************************************************************************/
void AudioSpeaker::KeyUnknownError(string key)
{
	string str = string("存在しないキーでの参照が要求されました。\n") + key;
	MessageBox(GetWindow(),
		TEXT(str.c_str()),
		TEXT("audioSpeaker:Error"),
		MB_OK | MB_ICONERROR);
}

/*******************************************************************************
*	再生情報リストに存在しないサウンド名称が渡された際のエラー処理
*******************************************************************************/
void AudioSpeaker::AudioNameUnknownError(string name)
{
	string str = string("再生情報リストに存在しないサウンド名称での参照が要求されました。\n") + name;
	MessageBox(GetWindow(),
		TEXT(str.c_str()),
		TEXT("audioSpeaker:Error"),
		MB_OK | MB_ICONERROR);
}

/*******************************************************************************
*	バッファサイズの標準値を取得
*******************************************************************************/
LONG AudioSpeaker::GetDefaultBufferLength(PlayInfo* info)
{
	// 1秒あたりのバイト数　＊　バッファあたりの秒数
	return info->AudioInfo->GetWaveFormat()->nAvgBytesPerSec * m_SECOND_PER_BUFFER;
}

/*******************************************************************************
*	ストリーミング用バッファを初期化
*******************************************************************************/
void AudioSpeaker::InitBuffer(PlayInfo* info)
{
	//------------------------
	// データバッファ読み込み
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
	// オーディオバッファ設定
	//------------------------
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = primaryBufLength;
	bufinfo.pAudioData = info->PrimaryBuffer;
	if (endOfSteramFlag)
	{
		// データの終端の場合は終了フラグを渡す
		bufinfo.Flags = XAUDIO2_END_OF_STREAM;
	}

	// バッファを反映
	info->SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);
}

/*******************************************************************************
*	確保可能なバッファサイズを計算
*******************************************************************************/
LONG AudioSpeaker::CalcBufferLength(PlayInfo* info)
{
	LONG defaultLen = GetDefaultBufferLength(info);
	LONG audioBytes = info->AudioInfo->GetAudioBytes();

	// 残りのサウンドデータ量よりバッファサイズの方が大きいかどうか
	if (audioBytes - info->SeekCursor < defaultLen)
	{// 大きい場合
		// 残りのサウンドデータ量を返す
		return audioBytes - info->SeekCursor;
	}
	else
	{// 大きくない場合
		// バッファサイズを返す
		return defaultLen;
	}
}

/*******************************************************************************
*	シーク位置が終端に達しているか確認する
*******************************************************************************/
bool AudioSpeaker::CheckSeek(PlayInfo* info)
{
	LONG audioBytes = info->AudioInfo->GetAudioBytes();

	// データの終端まで読み込みが終了しているかどうか
	if (info->SeekCursor >= audioBytes)
	{
		if (info->IsLoop)
		{
			// 最後まで読み込み済みの場合はシークを先頭に戻しループに備える
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
*	インスペクター表示処理
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


//　↓↓　コールバッククラスのメソッド　↓↓

/*******************************************************************************
*	XAUDIO2_END_OF_STREAMフラグを立てた状態で
* 	現在のバッファの最後のバイトが消費された直後に呼び出される
*******************************************************************************/
void AudioCallback::OnStreamEnd()
{
	// ループ設定ではない場合停止する
	if (!m_AudioSpeaker->GetPlayInfo_FromKey(m_Key)->IsLoop)
	{
		m_AudioSpeaker->Stop_FromKey(m_Key);
	}
}

/*******************************************************************************
*	現在のバッファの最後のバイトが消費された直後に呼び出される
*******************************************************************************/
void AudioCallback::OnBufferEnd(void* bufferContext)
{
	m_AudioSpeaker->AddNextBuffer(m_Key);
}
