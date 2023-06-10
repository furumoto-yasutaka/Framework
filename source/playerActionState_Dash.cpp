/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�_�b�V���X�e�[�g�N���X
*	�t�@�C���F	playerActionState_Dash.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerActionState_Dash.h"

PlayerActionState_Dash::PlayerActionState_Dash(PlayerManager* Manager)
	: PlayerActionStateBase(Manager, PlayerManager::ActionStateId::Dash)
{
	m_Param->m_MoveMagnitudeMax = m_DASH_MAGNITUDE;
	if (!m_Param->m_IsGun)
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Rod);
	}
	else
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Gun);
	}
}

void PlayerActionState_Dash::Update()
{
	CheckQueue(
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Move_Z].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Move_X].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Jump].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Avoid].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Roll_Left].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Roll_Right].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::WeaponDeformation].Tag
	);
	CheckEnd();
	CheckTurnOccur();

	UpdateMove();
	AddGravity();
	CalcResistance();
	CorrectionVelocity();
	RotateAngle();
}
void PlayerActionState_Dash::LateUpdate()
{
	AddVelocity();
}

void PlayerActionState_Dash::ExecMove_Z(PlayerInputListener::ActionElement* Elem)
{
	PlayerActionStateBase::ExecMove_Z(Elem);
	if (Elem->Value != 0.0f) { m_IsMoveInputZ = true; }
}
void PlayerActionState_Dash::ExecMove_X(PlayerInputListener::ActionElement* Elem)
{
	PlayerActionStateBase::ExecMove_X(Elem);
	if (Elem->Value != 0.0f) { m_IsMoveInputX = true; }
}

void PlayerActionState_Dash::CheckEnd()
{
	// ���͂��Ȃ��������Ԃ��L�^���A�w�莞�Ԉȏ�̓��͂����������疳���X�e�[�g�ɑJ�ڂ���
	if (!m_IsMoveInputX && !m_IsMoveInputZ)
	{
		m_InputEndTimeCount -= Time::GetDeltaTimeSec();
		if (m_InputEndTimeCount <= 0.0f)
		{
			m_Manager->RequestNextActionState(PlayerManager::ActionStateId::None);
		}
	}
	else
	{
		m_InputEndTimeCount = m_INPUT_END_TIME;
	}

	m_IsMoveInputZ = false;
	m_IsMoveInputX = false;
}
