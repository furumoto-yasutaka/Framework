/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�X�e�[�g���N���X
*	�t�@�C���F	eggEnemyStateBase.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "enemyStateBase.h"

class EggEnemyStateBase : public EnemyStateBase
{
protected:
	EggEnemyManager::AiStateId m_StateId;

public:
	EggEnemyStateBase(EggEnemyManager* Manager, EggEnemyManager::AiStateId StateId)
		: EnemyStateBase(Manager),
		m_StateId(StateId)
	{
		m_CommonParam->m_MoveMagnitudeMax = 0.0f;
	}
	virtual ~EggEnemyStateBase() {}

	virtual void Update() = 0;
	virtual void LateUpdate() = 0;

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	int GetStateId() override { return (int)m_StateId; }
	/////////////////////////////
};
