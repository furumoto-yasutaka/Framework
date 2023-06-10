/*******************************************************************************
*
*	タイトル：	たまごエネミー追跡アニメーションクラス
*	ファイル：	eggEnemyAnimationState_Approach.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "eggEnemyAnimationStateBase.h"

class EggEnemyAnimationState_Approach : public EggEnemyAnimationStateBase
{
public:
	EggEnemyAnimationState_Approach(EggEnemyManager* Manager, AnimationBaseParam Parameter)
		: EggEnemyAnimationStateBase(Manager, EggEnemyManager::AnimationStateId::Approach, Parameter)
	{}
	~EggEnemyAnimationState_Approach() {}

	void MainAnimation() override;
	void PreEndAnimation() override;
};
