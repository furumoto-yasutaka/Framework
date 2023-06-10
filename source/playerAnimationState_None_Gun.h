/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�����_�o���X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_None_Gun.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerAnimationStateBase.h"

class PlayerAnimationState_None_Gun : public PlayerAnimationStateBase
{
public:
	PlayerAnimationState_None_Gun(PlayerManager* Manager, AnimationBaseParam Parameter)
		: PlayerAnimationStateBase(Manager, PlayerManager::AnimationStateId::None_Gun, Parameter)
	{}
	~PlayerAnimationState_None_Gun() {}

	void MainAnimation() override;
	void PreEndAnimation() override;
};
