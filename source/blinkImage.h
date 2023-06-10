/*******************************************************************************
*
*	タイトル：	画像点滅スクリプト
*	ファイル：	blinkImage.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class BlinkImage : public NotDrawComponent
{
public:
	// 点滅方法
	enum class BlinkMode
	{
		None = 0,
		Fade,
	};
	// 点滅の進行状況
	enum class BlinkState
	{
		Visible = 0,
		ChangeInVisible,
		InVisible,
		ChangeVisible,
	};

private:
	BlinkMode	m_Mode = BlinkMode::None;			// 点滅方法
	BlinkState	m_State = BlinkState::Visible;		// 点滅の進行状況
	float		m_TimeCountSec = 0.0f;				// 時間計測(秒)
	float		m_VisibleTimeSec = 0.0f;			// 表示の継続時間
	float		m_InVisibleTimeSec = 0.0f;			// 非表示の継続時間
	
	float		m_ChangeVisibleTimeSec = 0.0f;		// 表示にかける時間
	float		m_ChangeInVisibleTimeSec = 0.0f;	// 非表示にかける時間

public:
	BlinkImage(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~BlinkImage() {}

	void Init(BlinkMode mode, float visibleTimeSec, float inVisibleTimeSec,
			float changeVisibleTimeSec = 0.0f, float changeInVisibleTimeSec = 0.0f);
	void Uninit() override {}
	void Update() override;

private:
	// 透明度変更
	void ChangeAlpha();
	// 透明度変更(フェードモード)
	void ChangeAlpha_Fade();

	// 点滅状況変更
	void ChangeState();
	// 点滅状況変更(無設定モード)
	void ChangeState_None();
	// 点滅状況変更(フェードモード)
	void ChangeState_Fade();
};
