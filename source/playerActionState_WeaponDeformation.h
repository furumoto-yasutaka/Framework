/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�_�o���X�e�[�g�N���X
*	�t�@�C���F	playerActionState_WeaponDeformation.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerActionStateBase.h"

class PlayerActionState_WeaponDeformation : public PlayerActionStateBase
{
public:
	PlayerActionState_WeaponDeformation(PlayerManager* Manager);
	~PlayerActionState_WeaponDeformation() {}

	void Update() override;
	void LateUpdate() override;
};
