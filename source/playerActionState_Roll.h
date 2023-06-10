/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���͈͉�]�X�e�[�g���N���X
*	�t�@�C���F	playerActionState_Roll.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerActionStateBase.h"

class PlayerActionState_Roll : public PlayerActionStateBase
{
private:
	static inline const float m_WALK_MAGNITUDE = 0.1f;	// �_�b�V�������x
	static inline const float m_JUMP_MAGNITUDE = 0.1f;	// �W�����v��
	static inline const float m_COOL_TIME = 0.3f;

public:
	PlayerActionState_Roll(PlayerManager* Manager, PlayerManager::ActionStateId ActionStateId);
	virtual ~PlayerActionState_Roll() {}

	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
};
