/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�����e�ό`�X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_WeaponDeformation_Gun.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerAnimationState_WeaponDeformation_Gun.h"
#include "gameObject.h"
#include "playerManager.h"

PlayerAnimationState_WeaponDeformation_Gun::~PlayerAnimationState_WeaponDeformation_Gun()
{
	// �A�j���[�V�����̏I�������}�ɃA�N�V�����X�e�[�g��ύX����
	m_Manager->RequestNextActionState(PlayerManager::ActionStateId::None);
}

void PlayerAnimationState_WeaponDeformation_Gun::MainAnimation()
{
	// �_���w��̍��W�܂Ŋ�����Ԃňړ�����
	GameObject* weapon = m_Manager->GetModelParts(PlayerManager::ModelPartsId::Weapon);
	float amount;
	bool isEnd = (m_ROTATE_LENGTH + m_ROTATE_SURPLUS_LENGTH) - m_RotatedLength <= m_ROTATE_THRESHOLD;
	
	if (isEnd)
	{
		amount = (m_ROTATE_LENGTH + m_ROTATE_SURPLUS_LENGTH) - m_RotatedLength;

		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Gun);
	}
	else
	{
		amount = (m_ROTATE_TARGET_LENGTH - m_RotatedLength) * m_ROTATE_INTERPOLATION_SPEED;
	}

	m_RotatedLength += amount;
	weapon->m_LocalRotation = Math::AxisRotation_Degree({ 1.0f, 0.0f, 0.0f }, m_RotatedLength);

	if (isEnd)
	{
		m_RotatedLength = 0.0f;
	}
}

void PlayerAnimationState_WeaponDeformation_Gun::PreEndAnimation()
{
	// �_���w��̍��W�܂Ŋ�����Ԃňړ�����
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
		m_ROTATE_LENGTH + m_ROTATE_SURPLUS_LENGTH - m_RotatedLength);
}