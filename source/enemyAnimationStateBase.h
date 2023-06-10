/*******************************************************************************
*
*	�^�C�g���F	�G�l�~�[�A�j���[�V�����X�e�[�g���N���X
*	�t�@�C���F	enemyAnimationStateBase.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"

class EnemyManager;

class EnemyAnimationStateBase
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
		bool IsBodyRotate;
		float RotateTargetAngle;
		float RotateAngle;
	};

protected:
	static inline const float m_ROTATE_ANGLE_MAX = 30.0f;			// �̂�����̍ő�p�x
	static inline const float m_ROTATE_ANGLE_RATE = 1000.0f;		// �ړ����x1������̂̂�����p�x
	static inline const float m_ROTATE_INTERPOLATION_RATE = 0.3f;	// �̂�����̉�]��ԑ��x

	EnemyManager* m_BaseManager = NULL;

	AnimationStepId	m_AnimationStepId = AnimationStepId::Main;

	AnimationBaseParam m_AnimationBaseParam;

public:
	EnemyAnimationStateBase(EnemyManager* Manager, AnimationBaseParam Parameter)
		: m_BaseManager(Manager),
		m_AnimationBaseParam(Parameter)
	{}
	virtual ~EnemyAnimationStateBase() {}

	virtual void Update();
	virtual void LateUpdate();

	virtual void MainAnimation() = 0;
	virtual void PreEndAnimation() = 0;

	virtual void PreEnd();	// �A�j���[�V�����̏I���A�j���[�V�����ɑJ�ڂ���
	virtual void End();		// �A�j���[�V�����̏I���ɑJ�ڂ���

	void UpdateBodyRotate(float MoveSpeed);	// �ړ��ɂ��J�o�[�̌X��

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	AnimationStepId GetAnimationStepId() { return m_AnimationStepId; }
	AnimationBaseParam GetAnimationBaseParam() { return m_AnimationBaseParam; }
	/////////////////////////////
};
