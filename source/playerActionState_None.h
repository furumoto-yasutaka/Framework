/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�����X�e�[�g�N���X
*	�t�@�C���F	playerActionState_None.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerActionStateBase.h"

class PlayerActionState_None : public PlayerActionStateBase
{
public:
	PlayerActionState_None(PlayerManager* Manager);
	~PlayerActionState_None() {}

	void Update() override;
	void LateUpdate() override;
};
