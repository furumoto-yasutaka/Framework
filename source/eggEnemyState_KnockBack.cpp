/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�m�b�N�o�b�N�X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_KnockBack.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "eggEnemyState_KnockBack.h"

EggEnemyState_KnockBack::EggEnemyState_KnockBack(EggEnemyManager* Manager)
	: EggEnemyStateBase(Manager, EggEnemyManager::AiStateId::KnockBack)
{
	m_CommonParam->m_MoveMagnitude = m_KNOCKBACK_MAGNITUDE;

	m_Manager->RequestNextAnimationState((int)EggEnemyManager::AnimationStateId::KnockBack);
}

void EggEnemyState_KnockBack::Update()
{
	UpdateMove();
	AddGravity();
	CalcResistance();
	CorrectionVelocity();
}

void EggEnemyState_KnockBack::LateUpdate()
{
	AddVelocity();
}
