/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�m�b�N�o�b�N�X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_KnockBack.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "eggEnemyStateBase.h"

class EggEnemyState_KnockBack : public EggEnemyStateBase
{
private:
	static inline const float m_KNOCKBACK_MAGNITUDE = 0.4f;

public:
	EggEnemyState_KnockBack(EggEnemyManager* Manager);
	~EggEnemyState_KnockBack() {}

	void Update() override;
	void LateUpdate() override;
};
