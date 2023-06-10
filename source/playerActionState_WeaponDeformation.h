/*******************************************************************************
*
*	タイトル：	プレイヤー棒出しステートクラス
*	ファイル：	playerActionState_WeaponDeformation.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "playerActionStateBase.h"

class PlayerActionState_WeaponDeformation : public PlayerActionStateBase
{
public:
	PlayerActionState_WeaponDeformation(PlayerManager* Manager);
	~PlayerActionState_WeaponDeformation() {}

	void Update() override;
	void LateUpdate() override;
};
