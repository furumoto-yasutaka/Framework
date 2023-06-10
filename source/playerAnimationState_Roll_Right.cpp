/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�������͈͉E��]�X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_Roll_Right.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerAnimationState_Roll_Right.h"
#include "playerManager.h"
#include "gameObject.h"
#include "camera.h"

PlayerAnimationState_Roll_Right::~PlayerAnimationState_Roll_Right()
{
	// �A�j���[�V�����̏I�������}�ɃA�N�V�����X�e�[�g��ύX����
	m_Manager->RequestNextActionState(PlayerManager::ActionStateId::None);
}

void PlayerAnimationState_Roll_Right::MainAnimation()
{
	MainScaleChange();
	MainRodRotate({ 0.0f, 1.0f, 0.0f });

	if (m_IsEndRotate && m_IsEndScaling)
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Rod);
		m_IsEndRotate = false;
		m_IsEndScaling = false;
	}
}
void PlayerAnimationState_Roll_Right::PreEndAnimation()
{
	PreEndScaleChange();
	PreEndRodRotate();

	if (m_IsEndRotate && m_IsEndScaling)
	{
		End();
	}
}
