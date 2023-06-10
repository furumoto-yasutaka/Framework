/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�����A�j���[�V�����N���X
*	�t�@�C���F	eggEnemyAnimationState_None.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "eggEnemyAnimationStateBase.h"

class EggEnemyAnimationState_None : public EggEnemyAnimationStateBase
{
public:
	EggEnemyAnimationState_None(EggEnemyManager* Manager, AnimationBaseParam Parameter)
		: EggEnemyAnimationStateBase(Manager, EggEnemyManager::AnimationStateId::None, Parameter)
	{}
	~EggEnemyAnimationState_None() {}

	void MainAnimation() override;
	void PreEndAnimation() override;
};
