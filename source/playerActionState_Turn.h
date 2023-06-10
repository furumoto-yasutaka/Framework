/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�U������X�e�[�g�N���X
*	�t�@�C���F	playerActionState_Turn.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerActionStateBase.h"

class PlayerActionState_Turn : public PlayerActionStateBase
{
private:
	static inline const float m_TURN_SPEED = 0.5f;					// �U��������x
	static inline const float m_LOW_MOVE_SPEED = 0.1f;				// �_�b�V�������x
	static inline const float m_TURN_END_ANGLE_THRESHOLD = 0.1f;	// �U��������I���Ƃ���p�x

public:
	PlayerActionState_Turn(PlayerManager* Manager);
	~PlayerActionState_Turn() {}

	void Update() override;
	void LateUpdate() override;

	void CheckTurnAngle();	// �U��������I�����邩�m�F
};
