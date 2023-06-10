/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[���˂��U���A�j���[�V�����N���X
*	�t�@�C���F	eggEnemyAnimationState_HeadAttack.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "eggEnemyAnimationState_HeadAttack.h"

void EggEnemyAnimationState_HeadAttack::MainAnimation()
{
	switch (m_State)
	{
		using enum State;
	case Charge:
		break;
	case Attack:
		break;
	}
}

void EggEnemyAnimationState_HeadAttack::PreEndAnimation()
{
	End();
}

void EggEnemyAnimationState_HeadAttack::ChargeAnimation()
{
	float amount = m_KNOCK_DOWN_ANGLE - m_KnockDownAngle;
	if (amount < m_KNOCK_DOWN_INTERPOLATION_RATE)
	{
		amount *= m_KNOCK_DOWN_INTERPOLATION_RATE;
	}
	m_KnockDownAngle += amount;

	m_ShrinkedScale += m_SHRINK_SPEED;
	float result = 1.0f - m_ShrinkedScale;
	m_Manager->GetAttachObject()->m_LocalScale = { result, result, result };
}

void EggEnemyAnimationState_HeadAttack::AttackAnimation()
{

}
