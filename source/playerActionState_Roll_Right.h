/*******************************************************************************
*
*	タイトル：	プレイヤー中範囲右回転ステートクラス
*	ファイル：	playerActionState_Roll_Right.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "playerActionState_Roll.h"

class PlayerActionState_Roll_Right : public PlayerActionState_Roll
{
public:
	PlayerActionState_Roll_Right(PlayerManager* Manager);
	~PlayerActionState_Roll_Right() {}

	void Update() override;
	void LateUpdate() override;
};
