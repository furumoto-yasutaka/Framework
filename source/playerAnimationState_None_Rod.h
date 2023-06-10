/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�����ҋ@�X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_None_Rod.h
*	�쐬�ҁF	�Ö{ �ח�
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
