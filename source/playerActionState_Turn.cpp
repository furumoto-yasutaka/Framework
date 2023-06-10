/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�U������X�e�[�g�N���X
*	�t�@�C���F	playerActionState_Turn.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerActionState_Turn.h"
#include "collisionContactRecorder_PlayerGround.h"

PlayerActionState_Turn::PlayerActionState_Turn(PlayerManager* Manager)
	: PlayerActionStateBase(Manager, PlayerManager::ActionStateId::Turn)
{
	m_Param->m_TurnSpeed = m_TURN_SPEED;
	m_Param->m_MoveMagnitudeMax = m_LOW_MOVE_SPEED;
	m_Param->m_MoveMagnitude = 0.0f;
	if (!m_Param->m_IsGun)
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Rod);
	}
	else
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Gun);
	}
}

void PlayerActionState_Turn::Update()
{
	CheckQueue(
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Jump].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Avoid].Tag
	);
	CheckTurnAngle();

	UpdateMove();
	AddGravity();
	CalcResistance();
	CorrectionVelocity();
	RotateAngle();
}
void PlayerActionState_Turn::LateUpdate()
{
	AddVelocity();
}

void PlayerActionState_Turn::CheckTurnAngle()
{
	if (m_Param->m_MoveAngleDistance <= m_TURN_END_ANGLE_THRESHOLD)
	{
		if (m_Manager->GetRecvActionStateId() == PlayerManager::ActionStateId::Dash)
		{
			m_Manager->RequestNextActionState(PlayerManager::ActionStateId::Dash);
		}
		else
		{
			m_Manager->RequestNextActionState(PlayerManager::ActionStateId::None);
		}
	}
}
