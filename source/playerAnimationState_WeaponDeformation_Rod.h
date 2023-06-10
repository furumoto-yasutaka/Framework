/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�����_�[�߃X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_WeaponDeformation_Rod.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerAnimationStateBase.h"

class PlayerAnimationState_WeaponDeformation_Rod : public PlayerAnimationStateBase
{
private:
	static inline const float m_ROTATE_LENGTH = 180.0f;
	static inline const float m_ROTATE_SURPLUS_LENGTH = 10.0f;
	static inline const float m_ROTATE_TARGET_LENGTH = 250.0f;
	static inline const float m_ROTATE_THRESHOLD = 0.1f;
	static inline const float m_ROTATE_INTERPOLATION_SPEED = 0.1f;
	static inline const float m_END_ROTATE_INTERPOLATION_SPEED = 0.01f;

	float m_RotatedLength = 0.0f;

public:
	PlayerAnimationState_WeaponDeformation_Rod(PlayerManager* Manager, AnimationBaseParam Parameter)
		: PlayerAnimationStateBase(Manager, PlayerManager::AnimationStateId::WeaponDeformation_Rod, Parameter)
	{}
	~PlayerAnimationState_WeaponDeformation_Rod() override;

	void MainAnimation() override;
	void PreEndAnimation() override;
};
