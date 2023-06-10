/*******************************************************************************
*
*	タイトル：	サウンド再生用コンポーネント
*	ファイル：	audioSpeaker.h
*	作成者：	古本 泰隆
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
	// 再生用の情報をまとめた構造体
	struct PlayInfo
	{
		string					AudioName;			// サウンドの名称(コンテナ上の名前)
		AudioInfo*				AudioInfo;			// サウンドの詳細情報
		AudioInfo::AudioState	State;				// サウンドの状態
		bool					IsLoop;				// ループの有無
		float					Volume;				// 音量
		IXAudio2SourceVoice*	SourceVoice;		// スピーカー

		// ストリーミング再生時のみ使用
		HMMIO*					Hmmio;				// 読み込み用ハンドル
		AudioCallback*			CallBack;			// コールバッククラス
		BYTE*					PrimaryBuffer;		// バッファ1
		BYTE*					SecondaryBuffer;	// バッファ2
		LONG					SecondaryBufLength;	// バッファ2のうち使用している領域
		LONG					SeekCursor;			// ファイルの読み込み位置
	};

private:
	static inline list<AudioSpeaker*>	m_SpeakerList;				// サウンドの全体操作用インスタンスリスト
	static inline const unsigned int	m_SECOND_PER_BUFFER = 1;	// ストリーミング再生用のバッファサイズをサウンドの何秒確保するか

	// ▼要素
	// 引数1	string型	識別キー(サウンド名称(連番))
	// 引数2	PlayInfo*型	再生情報
	map<string, PlayInfo*>	m_PlayInfo;		// 再生情報リスト

public:
	AudioSpeaker(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~AudioSpeaker() {}

	void Init();
	void Uninit() override;
	void Update() override {}

	// 再生開始(戻り値は操作用の識別キー)
	string Play(string name, bool isLoop = false, float volume = 1.0f);

	// 再生停止(キー参照)
	void Stop_FromKey(string key);

	// 再生停止(名称参照 ※複数ある場合は現在再生中の中で最も連番が小さいサウンド)
	void Stop_FromAudioName(string name);

	// 再生再開
	void Resume(string key);

	// 一時停止
	void Pause(string key);

	// 音量設定
	void SetVolume(string key, float volume);

	/////////////////////////////////////////////////
	//　↓↓　ストリーミング再生用メソッド　↓↓　//

	// 追加のバッファを読み込む
	void AddNextBuffer(string key);

	// 再生情報を取得(キー参照)
	PlayInfo* GetPlayInfo_FromKey(string key) { return m_PlayInfo[key]; }

	/////////////////////////////////////////////////
private:
	// サウンドを登録
	PlayInfo* SetAudio(string key, string name, bool isLoop, float volume);

	// 再生情報のイテレータを取得
	// (名称参照 ※複数ある場合は現在再生中の中で最も連番が小さいサウンド)
	map<string, PlayInfo*>::iterator GetIterator_FromAudioName(string name);
	
	// 有効でないキーが渡された際のエラー処理
	void KeyUnknownError(string key);

	// 再生情報リストに存在しないサウンド名称が渡された際のエラー処理
	void AudioNameUnknownError(string name);

	/////////////////////////////////////////////////
	//　↓↓　ストリーミング再生用メソッド　↓↓　//
	
	// バッファサイズの標準値を取得
	LONG GetDefaultBufferLength(PlayInfo* info);

	// ストリーミング用バッファを初期化
	void InitBuffer(PlayInfo* info);

	// 確保可能なバッファサイズを計算
	LONG CalcBufferLength(PlayInfo* info);

	// シーク位置が終端に達しているか確認する
	bool CheckSeek(PlayInfo* info);
	/////////////////////////////////////////////////

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	map<string, PlayInfo*> GetPlayInfo() { return m_PlayInfo; }
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};

//--------------------------------------------
// ストリーミング再生用コールバッククラス
//--------------------------------------------
class AudioCallback : public IXAudio2VoiceCallback
{
private:
	AudioSpeaker*	m_AudioSpeaker;	// スピーカー
	string			m_Key;			// スピーカーに登録されたサウンドの識別キー

public:
	AudioCallback(AudioSpeaker* audioSpeaker, string key)
		: m_AudioSpeaker(audioSpeaker), m_Key(key) {}
	~AudioCallback() {}

	// ????
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) {}
	// ????
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) {}
	// XAUDIO2_END_OF_STREAMフラグを立てた状態で
	// 現在のバッファの最後のバイトが消費された直後に呼び出される
	void STDMETHODCALLTYPE OnStreamEnd(void);
	// 現在のバッファの最初のバイトが消費される直前に呼び出される
	void STDMETHODCALLTYPE OnBufferStart(void* bufferContext) {}
	// 現在のバッファの最後のバイトが消費された直後に呼び出される
	void STDMETHODCALLTYPE OnBufferEnd(void* bufferContext);
	// ループの終了地点に達したときに呼び出される
	void STDMETHODCALLTYPE OnLoopEnd(void* bufferContext) {}
	// サウンドに何らかのエラーが発生した時に呼び出される(？)
	void STDMETHODCALLTYPE OnVoiceError(void* bufferContext, HRESULT error) {}
};
