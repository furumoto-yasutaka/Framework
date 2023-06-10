/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�p�j�X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_Search.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "eggEnemyStateBase.h"

class EggEnemyState_Search : public EggEnemyStateBase
{
private:
	enum class SearchStepId
	{
		Wait = 0,
		Move,
	};

	static inline const float m_WAIT_TIME_MAX = 3.0f;
	static inline const float m_WAIT_TIME_MIN = 1.0f;
	static inline const float m_MOVE_TIME_MAX = 2.0f;
	static inline const float m_MOVE_TIME_MIN = 0.5f;
	static inline const float m_MOVE_MAGNITUDE_MAX = 0.07f;
	static inline const int	m_MOVE_CONSECUTIVE_NUM = 3;		// �ő剽�A���ňړ���I�����邩
	static inline const float m_MOVE_SELECT_RATE = 0.6f;	// �ړ���Ɉړ���I������m��
	static inline const D3DXVECTOR3 m_SEARCH_RANGE = { 12.0f, 10.0f, 15.0f };	// �T�m�͈�
	//--------------------------------
	// ��
	static inline const float m_SEARCH_RANGE2 = 15.0f;
	//--------------------------------

	SearchStepId m_SearchStepId = SearchStepId::Wait;
	float m_StepTimeLimit = 0.0f;
	int m_MoveConsecutiveCount = 0;

public:
	EggEnemyState_Search(EggEnemyManager* Manager);
	~EggEnemyState_Search() {}

	void Update() override;
	void LateUpdate() override;

	void Search();

	void ChangeStep();
	void ChangeWait();
	void ChangeMove();

	D3DXVECTOR3 CreateAngle();	// �V�����ړ������𐶐�
};
