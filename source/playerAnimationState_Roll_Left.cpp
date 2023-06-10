/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�������͈͍���]�X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_Roll_Left.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerAnimationState_Roll_Left.h"
#include "playerManager.h"
#include "gameObject.h"
#include "camera.h"

PlayerAnimationState_Roll_Left::~PlayerAnimationState_Roll_Left()
{
	// �A�j���[�V�����̏I�������}�ɃA�N�V�����X�e�[�g��ύX����
	m_Manager->RequestNextActionState(PlayerManager::ActionStateId::None);
}

void PlayerAnimationState_Roll_Left::MainAnimation()
{
	MainScaleChange();
	MainRodRotate({ 0.0f, -1.0f, 0.0f });

	if (m_IsEndRotate && m_IsEndScaling)
	{
		m_Manager->RequestNextAnimationState(PlayerManager::AnimationStateId::None_Rod);
		m_IsEndRotate = false;
		m_IsEndScaling = false;
	}
}
void PlayerAnimationState_Roll_Left::PreEndAnimation()
{
	PreEndScaleChange();
	PreEndRodRotate();

	if (m_IsEndRotate && m_IsEndScaling)
	{
		End();
	}
}
