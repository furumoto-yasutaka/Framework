/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[���˂��U���A�j���[�V�����N���X
*	�t�@�C���F	eggEnemyAnimationState_HeadAttack.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "eggEnemyAnimationStateBase.h"

class EggEnemyAnimationState_HeadAttack : public EggEnemyAnimationStateBase
{
public:
	enum class State
	{
		Charge = 0,
		Attack,
	};

private:
	static inline const float m_KNOCK_DOWN_ANGLE = 70.0f;
	static inline const float m_KNOCK_DOWN_INTERPOLATION_RATE = 0.1f;
	
	static inline const float m_SHRINK = 0.2f;
	static inline const float m_SHRINK_SPEED = 0.003f;

	State m_State = State::Charge;

	float m_KnockDownAngle = 0.0f;
	float m_ShrinkedScale = 0.0f;

public:
	EggEnemyAnimationState_HeadAttack(EggEnemyManager* Manager, AnimationBaseParam Parameter)
		: EggEnemyAnimationStateBase(Manager, EggEnemyManager::AnimationStateId::HeadAttack, Parameter)
	{}
	~EggEnemyAnimationState_HeadAttack() {}

	void MainAnimation() override;
	void PreEndAnimation() override;

	void ChargeAnimation();
	void AttackAnimation();
};
