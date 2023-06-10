/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�Ђ�݃X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_Wince.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "eggEnemyState_Wince.h"

void EggEnemyState_Wince::Update()
{
	if (m_TimeLimit <= 0.0f)
	{
		m_Manager->RequestNextAiState((int)EggEnemyManager::AiStateId::Search);
	}
	else
	{
		m_TimeLimit -= Time::GetDeltaTimeSec();
	}

	AddGravity();
	CalcResistance();
	CorrectionVelocity();
}

void EggEnemyState_Wince::LateUpdate()
{
	AddVelocity();
}
