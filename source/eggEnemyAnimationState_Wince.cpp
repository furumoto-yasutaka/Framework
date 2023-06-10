/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[���݃A�j���[�V�����N���X
*	�t�@�C���F	eggEnemyAnimationState_Wince.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "eggEnemyAnimationState_Wince.h"

EggEnemyAnimationState_Wince::EggEnemyAnimationState_Wince(EggEnemyManager* Manager, AnimationBaseParam Parameter)
	: EggEnemyAnimationStateBase(Manager, EggEnemyManager::AnimationStateId::Wince, Parameter)
{
	m_Manager->GetModelRoot()->m_LocalRotation = Math::GetQuaternionIdentity();
}

void EggEnemyAnimationState_Wince::MainAnimation()
{
	float amount = m_SPIN_RADIUS - m_SpinRadius;
	if (amount > m_SPIN_RADIUS_INTERPOLATION_THRESHOLD)
	{
		amount *= m_SPIN_RADIUS_INTERPOLATION_RATE;
	}
	m_SpinRadius += amount;

	float time = Time::GetDeltaTimeSec() * m_SPIN_SPEED;
	float rotateX = (cosf(time) + m_SPIN_DEFORMATION_X) * m_SpinRadius;
	float rotateZ = (sinf(time) + m_SPIN_DEFORMATION_Z) * m_SpinRadius;

	GameObject* model = m_Manager->GetModelRoot();
	model->m_LocalRotation = Math::GetQuaternionIdentity();
	model->m_LocalRotation = Math::AxisRotation_Degree({ 1.0f, 0.0f, 0.0f }, rotateX);
	model->m_LocalRotation = Math::AxisRotation_Degree({ 0.0f, 0.0f, 1.0f }, rotateZ);
}

void EggEnemyAnimationState_Wince::PreEndAnimation()
{
	End();
}
