/*******************************************************************************
*
*	タイトル：	プレイヤー中範囲左回転ステートクラス
*	ファイル：	playerActionState_Roll_Left.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "playerActionState_Roll.h"

class PlayerActionState_Roll_Left : public PlayerActionState_Roll
{
public:
	PlayerActionState_Roll_Left(PlayerManager* Manager);
	~PlayerActionState_Roll_Left() {}

	void Update() override;
	void LateUpdate() override;
};
