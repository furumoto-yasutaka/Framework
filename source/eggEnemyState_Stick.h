/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�����t���X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_Stick.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "eggEnemyStateBase.h"

class EggEnemyState_Stick : public EggEnemyStateBase
{
private:


public:
	EggEnemyState_Stick(EggEnemyManager* Manager)
		: EggEnemyStateBase(Manager, EggEnemyManager::AiStateId::Stick)
	{}
	~EggEnemyState_Stick() {}

	void Update() override {}
	void LateUpdate() override {}
};
