/*******************************************************************************
*
*	タイトル：	プレイヤー入力受付スクリプト
*	ファイル：	playerInputListener_MouseKeyboard.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "playerInputListener.h"

#include <optional>

class PlayerInputListener_MouseKeyboard : public PlayerInputListener
{
public:
	PlayerInputListener_MouseKeyboard(PlayerManager* Manager)
		: PlayerInputListener(Manager)
	{}
	~PlayerInputListener_MouseKeyboard() {}

	// 入力取得
	float InputMove_Z() override;			// 移動入力(前後)
	float InputMove_X() override;			// 移動入力(左右)
	bool InputJump() override;				// ジャンプ入力
	bool InputAvoid() override;				// 回避入力
	optional<float> InputRoll_L() override;	// 中範囲回転入力
	optional<float> InputRoll_R() override;	// 中範囲回転入力
	bool InputWeaponDeformation() override;	// 武器変形入力
};
