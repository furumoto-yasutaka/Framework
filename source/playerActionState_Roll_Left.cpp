/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���͈͍���]�X�e�[�g�N���X
*	�t�@�C���F	playerActionState_Roll_Left.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerActionState_Roll_Left.h"

PlayerActionState_Roll_Left::PlayerActionState_Roll_Left(PlayerManager* Manager)
	: PlayerActionState_Roll(Manager, PlayerManager::ActionStateId::Roll_Left)
{
	m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::Roll_Left);
}

void PlayerActionState_Roll_Left::Update()
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
void PlayerActionState_Roll_Left::LateUpdate()
{
	AddVelocity();
}
