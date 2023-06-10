/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�������͈͍���]�X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_Roll_Left.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerAnimationState_Roll.h"

class PlayerAnimationState_Roll_Left : public PlayerAnimationState_Roll
{
public:
	PlayerAnimationState_Roll_Left(PlayerManager* Manager, AnimationBaseParam Parameter)
		: PlayerAnimationState_Roll(Manager, PlayerManager::AnimationStateId::Roll_Left, Parameter)
	{}
	~PlayerAnimationState_Roll_Left() override;

	void MainAnimation() override;
	void PreEndAnimation() override;
};
