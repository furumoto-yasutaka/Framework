/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�Ђ�݃X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_Wince.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "eggEnemyStateBase.h"

class EggEnemyState_Wince : public EggEnemyStateBase
{
private:
	static inline const float m_WINCE_TIME = 7.0f;

	float m_TimeLimit = m_WINCE_TIME;

public:
	EggEnemyState_Wince(EggEnemyManager* Manager)
		: EggEnemyStateBase(Manager, EggEnemyManager::AiStateId::Wince)
	{}
	~EggEnemyState_Wince() {}

	void Update() override;
	void LateUpdate() override;
};
