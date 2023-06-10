/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���͎�t�X�N���v�g
*	�t�@�C���F	playerInputListener_MouseKeyboard.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerInputListener.h"

#include <optional>

class PlayerInputListener_MouseKeyboard : public PlayerInputListener
{
public:
	PlayerInputListener_MouseKeyboard(PlayerManager* Manager)
		: PlayerInputListener(Manager)
	{}
	~PlayerInputListener_MouseKeyboard() {}

	// ���͎擾
	float InputMove_Z() override;			// �ړ�����(�O��)
	float InputMove_X() override;			// �ړ�����(���E)
	bool InputJump() override;				// �W�����v����
	bool InputAvoid() override;				// ������
	optional<float> InputRoll_L() override;	// ���͈͉�]����
	optional<float> InputRoll_R() override;	// ���͈͉�]����
	bool InputWeaponDeformation() override;	// ����ό`����
};