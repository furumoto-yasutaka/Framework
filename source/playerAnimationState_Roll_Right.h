/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�������͈͉E��]�X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_Roll_Right.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerAnimationState_Roll.h"

class PlayerAnimationState_Roll_Right : public PlayerAnimationState_Roll
{
public:
	PlayerAnimationState_Roll_Right(PlayerManager* Manager, AnimationBaseParam Parameter)
		: PlayerAnimationState_Roll(Manager, PlayerManager::AnimationStateId::Roll_Right, Parameter)
	{}
	~PlayerAnimationState_Roll_Right() override;

	void MainAnimation() override;
	void PreEndAnimation() override;
};
