/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�����X�e�[�g�N���X
*	�t�@�C���F	playerActionState_None.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerActionState_None.h"
#include "collisionContactRecorder_PlayerGround.h"

PlayerActionState_None::PlayerActionState_None(PlayerManager* Manager)
	: PlayerActionStateBase(Manager, PlayerManager::ActionStateId::None)
{
	if (!m_Param->m_IsGun)
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Rod);
	}
	else
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Gun);
	}
}

void PlayerActionState_None::Update()
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
	CheckTurnOccur();

	UpdateMove();
	AddGravity();
	CalcResistance();
	CorrectionVelocity();
	RotateAngle();
}
void PlayerActionState_None::LateUpdate()
{
	AddVelocity();
}
