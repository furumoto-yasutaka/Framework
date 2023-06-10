/*******************************************************************************
*
*	タイトル：	デバッグ用カメラ制御スクリプト
*	ファイル：	debugCameraControl.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class DebugCameraControl : public NotDrawComponent
{
private:
	float		m_RotateSenseX = 0.15f;		// 回転感度(X)
	float		m_RotateSenseY = 0.1f;		// 回転感度(Y)
	float		m_RotateMaxX = 60.0f;		// 回転上限(X)
	float		m_MoveSpeedKey = 0.15f;		// 移動速度(キー)
	float		m_MoveSpeedDrag = 0.05f;	// 移動速度(マウスドラッグ)
	float		m_MoveSpeedWheel = 0.05f;	// 移動速度(マウスホイール)

	float		m_RotateXValue = 0.0f;	// ローカルX方向への回転量
	float		m_RotateYValue = 0.0f;	// ローカルY方向への回転量

public:
	DebugCameraControl(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~DebugCameraControl() {}

	void Init() {}
	void Uninit() override {}
	void Update() override;
};