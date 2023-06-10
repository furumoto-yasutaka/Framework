/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[���S�X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_Death.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "eggEnemyStateBase.h"

class EggEnemyState_Death : public EggEnemyStateBase
{
private:


public:
	EggEnemyState_Death(EggEnemyManager* Manager)
		: EggEnemyStateBase(Manager, EggEnemyManager::AiStateId::Death)
	{}
	~EggEnemyState_Death() {}

	void Update() override {}
	void LateUpdate() override {}
};
