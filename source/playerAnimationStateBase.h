/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�����X�e�[�g���N���X
*	�t�@�C���F	playerAnimationStateBase.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "playerManager.h"

class PlayerAnimationStateBase
{
public:
	enum class AnimationStepId
	{
		Main = 0,
		PreEnd,
		End,
	};
	struct AnimationBaseParam
	{
		float CoverRotateTargetAngle = 0.0f;
		float CoverRotateAngle = 0.0f;
	};

protected:
	static inline const float m_CAMERA_FOLLOW_ROTATE_INTERPOLATION_RATE = 0.3f;

	static inline const float m_SPHERE_ROTATE_SPEED = 30.0f;				// �ړ����x1������̋ʂ̉�]���x
	static inline const float m_COVER_ROTATE_ANGLE_MAX = 45.0f;				// �J�o�[�̍ő�p�x
	static inline const float m_COVER_ROTATE_ANGLE_RATE = 60.0f;			// �ړ����x1������̃J�o�[�̊p�x
	static inline const float m_COVER_ROTATE_INTERPOLATION_RATE = 0.3f;		// �J�o�[�̉�]��ԑ��x

	PlayerManager*					m_Manager = NULL;

	PlayerManager::AnimationStateId m_AnimationStateId;
	AnimationStepId					m_AnimationStepId = AnimationStepId::Main;
	bool							m_IsCameraRotate = true;	// �J�����̉�]��_�ڑ������ɖ��������邩

	AnimationBaseParam m_AnimationBaseParam;

public:
	PlayerAnimationStateBase(
		PlayerManager* Manager, PlayerManager::AnimationStateId AnimationStateId, AnimationBaseParam Parameter)
		: m_Manager(Manager),
		m_AnimationStateId(AnimationStateId),
		m_AnimationBaseParam(Parameter)
	{}
	virtual ~PlayerAnimationStateBase() {}

	virtual void Update();
	virtual void LateUpdate();

	virtual void MainAnimation() = 0;
	virtual void PreEndAnimation() = 0;

	void PreEnd();	// �A�j���[�V�����̏I���A�j���[�V�����ɑJ�ڂ���
	void End();		// �A�j���[�V�����̏I���ɑJ�ڂ���

	void UpdateParentRotateLock();
	void UpdateCameraRotate();

	void UpdateSphereRotate(float MoveSpeed);	// �ړ��ɂ�鋅�̉�]
	void UpdateCoverRotate(float MoveSpeed);	// �ړ��ɂ��J�o�[�̌X��

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	PlayerManager::AnimationStateId GetStateId() { return m_AnimationStateId; }
	AnimationStepId GetAnimationStepId() { return m_AnimationStepId; }
	AnimationBaseParam GetAnimationBaseParam() { return m_AnimationBaseParam; }
	/////////////////////////////
};
