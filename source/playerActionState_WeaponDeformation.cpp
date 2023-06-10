/*******************************************************************************
*
*	タイトル：	プレイヤー棒出しステートクラス
*	ファイル：	playerActionState_WeaponDeformation.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "playerActionState_WeaponDeformation.h"

PlayerActionState_WeaponDeformation::PlayerActionState_WeaponDeformation(PlayerManager* Manager)
	: PlayerActionStateBase(Manager, PlayerManager::ActionStateId::WeaponDeformation)
{
	if (m_Param->m_IsGun)
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::WeaponDeformation_Rod);
	}
	else
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::WeaponDeformation_Gun);
	}

	m_Param->m_IsGun = !m_Param->m_IsGun;
}

void PlayerActionState_WeaponDeformation::Update()
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
void PlayerActionState_WeaponDeformation::LateUpdate()
{
	AddVelocity();
}
