/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���͈͍���]�X�e�[�g�N���X
*	�t�@�C���F	playerActionState_Roll_Left.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerActionState_Roll.h"

class PlayerActionState_Roll_Left : public PlayerActionState_Roll
{
public:
	PlayerActionState_Roll_Left(PlayerManager* Manager);
	~PlayerActionState_Roll_Left() {}

	void Update() override;
	void LateUpdate() override;
};
