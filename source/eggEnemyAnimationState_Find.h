/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�����A�j���[�V�����N���X
*	�t�@�C���F	eggEnemyAnimationState_Find.h
*	�쐬�ҁF	�Ö{ �ח�
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
