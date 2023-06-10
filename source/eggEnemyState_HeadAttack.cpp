/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�U���X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_HeadAttack.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "eggEnemyState_HeadAttack.h"

EggEnemyState_HeadAttack::EggEnemyState_HeadAttack(EggEnemyManager* Manager)
	: EggEnemyStateBase(Manager, EggEnemyManager::AiStateId::Attack)
{
	// �U���p�̓����蔻���L���ɂ���
}

void EggEnemyState_HeadAttack::Update()
{
	UpdateMove();
	AddGravity();
	CalcResistance();
	CorrectionVelocity();
	RotateAngle();
}

void EggEnemyState_HeadAttack::LateUpdate()
{
	AddVelocity();
}

void EggEnemyState_HeadAttack::Attack()
{
	// �I�u�W�F�N�g�ɐݒ肳��Ă��铖���蔻���
	// ���ݏՓ˂��Ă���I�u�W�F�N�g���m�F���A
	// �v���C���[���܂܂�Ă�����_���[�W��^����
}
