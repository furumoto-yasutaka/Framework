/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���͈͉E��]�X�e�[�g�N���X
*	�t�@�C���F	playerActionState_Roll_Right.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerActionState_Roll.h"

class PlayerActionState_Roll_Right : public PlayerActionState_Roll
{
public:
	PlayerActionState_Roll_Right(PlayerManager* Manager);
	~PlayerActionState_Roll_Right() {}

	void Update() override;
	void LateUpdate() override;
};
