/*******************************************************************************
*
*	タイトル：	プレイヤーアニメーション棒納めステートクラス
*	ファイル：	playerAnimationState_WeaponDeformation_Rod.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "playerAnimationState_WeaponDeformation_Rod.h"
#include "gameObject.h"
#include "playerManager.h"

PlayerAnimationState_WeaponDeformation_Rod::~PlayerAnimationState_WeaponDeformation_Rod()
{
	// アニメーションの終了を合図にアクションステートを変更する
	m_Manager->RequestNextActionState(PlayerManager::ActionStateId::None);
}

void PlayerAnimationState_WeaponDeformation_Rod::MainAnimation()
{
	// 棒を指定の座標まで割合補間で移動する
	GameObject* weapon = m_Manager->GetModelParts(PlayerManager::ModelPartsId::Weapon);
	float amount;
	bool isEnd = (m_ROTATE_LENGTH + m_ROTATE_SURPLUS_LENGTH) - m_RotatedLength <= m_ROTATE_THRESHOLD;

	if (isEnd)
	{
		amount = (m_ROTATE_LENGTH + m_ROTATE_SURPLUS_LENGTH) - m_RotatedLength;

		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Rod);
	}
	else
	{
		amount = (m_ROTATE_TARGET_LENGTH - m_RotatedLength) * m_ROTATE_INTERPOLATION_SPEED;
	}

	m_RotatedLength += amount;
	weapon->m_LocalRotation = Math::AxisRotation_Degree({ 1.0f, 0.0f, 0.0f }, m_ROTATE_LENGTH - m_RotatedLength);

	if (isEnd)
	{
		m_RotatedLength = 0.0f;
	}
}

void PlayerAnimationState_WeaponDeformation_Rod::PreEndAnimation()
{
	// 棒を指定の座標まで割合補間で移動する
	GameObject* weapon = m_Manager->GetModelParts(PlayerManager::ModelPartsId::Weapon);
	float amount;

	if (m_ROTATE_SURPLUS_LENGTH - m_RotatedLength <= m_ROTATE_THRESHOLD)
	{
		amount = m_ROTATE_SURPLUS_LENGTH - m_RotatedLength;

		End();
	}
	else
	{
		amount = (m_ROTATE_TARGET_LENGTH - m_RotatedLength) * m_END_ROTATE_INTERPOLATION_SPEED;
	}

	m_RotatedLength += amount;
	weapon->m_LocalRotation = Math::AxisRotation_Degree({ 1.0f, 0.0f, 0.0f },
		m_RotatedLength - m_ROTATE_SURPLUS_LENGTH);
}
