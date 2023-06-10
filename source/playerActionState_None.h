/*******************************************************************************
*
*	タイトル：	プレイヤー無しステートクラス
*	ファイル：	playerActionState_None.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "playerActionStateBase.h"

class PlayerActionState_None : public PlayerActionStateBase
{
public:
	PlayerActionState_None(PlayerManager* Manager);
	~PlayerActionState_None() {}

	void Update() override;
	void LateUpdate() override;
};
