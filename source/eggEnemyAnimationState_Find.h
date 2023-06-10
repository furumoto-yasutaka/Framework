/*******************************************************************************
*
*	タイトル：	たまごエネミー発見アニメーションクラス
*	ファイル：	eggEnemyAnimationState_Find.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "eggEnemyAnimationStateBase.h"

class EggEnemyAnimationState_Find : public EggEnemyAnimationStateBase
{
public:
	EggEnemyAnimationState_Find(EggEnemyManager* Manager, AnimationBaseParam Parameter)
		: EggEnemyAnimationStateBase(Manager, EggEnemyManager::AnimationStateId::Find, Parameter)
	{}
	~EggEnemyAnimationState_Find() {}

	void MainAnimation() override;
	void PreEndAnimation() override;
};
