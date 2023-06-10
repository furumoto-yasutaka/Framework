/*******************************************************************************
*
*	タイトル：	プレイヤーアニメーション待機ステートクラス
*	ファイル：	playerAnimationState_None_Rod.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "playerAnimationStateBase.h"

class PlayerAnimationState_None_Rod : public PlayerAnimationStateBase
{
public:
	PlayerAnimationState_None_Rod(PlayerManager* Manager, AnimationBaseParam Parameter)
		: PlayerAnimationStateBase(Manager, PlayerManager::AnimationStateId::None_Rod, Parameter)
	{}
	~PlayerAnimationState_None_Rod() {}

	void MainAnimation() override;
	void PreEndAnimation() override;
};
