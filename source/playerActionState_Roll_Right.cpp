/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���͈͉E��]�X�e�[�g�N���X
*	�t�@�C���F	playerActionState_Roll_Right.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerActionState_Roll_Right.h"

PlayerActionState_Roll_Right::PlayerActionState_Roll_Right(PlayerManager* Manager)
	: PlayerActionState_Roll(Manager, PlayerManager::ActionStateId::Roll_Right)
{
	m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::Roll_Right);
}

void PlayerActionState_Roll_Right::Update()
{
	CheckQueue(
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Move_Z].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Move_X].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Jump].Tag |
		PlayerInputListener::m_ACTION_INFO[(int)PlayerInputListener::ActionId::Avoid].Tag
	);

	UpdateMove();
	AddGravity();
	CalcResistance();
	CorrectionVelocity();
	RotateAngle();
}
void PlayerActionState_Roll_Right::LateUpdate()
{
	AddVelocity();
}
