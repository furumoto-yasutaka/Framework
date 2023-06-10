/*******************************************************************************
*
*	�^�C�g���F	�G�l�~�[�X�e�[�g���N���X
*	�t�@�C���F	enemyStateBase.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "eggEnemyManager.h"

class EnemyManager;
class EnemyCommonParameter;

class EnemyStateBase
{
protected:
	static inline const float m_VELOCITY_MAX_XZ = 1.0f;						// �ő�ړ����x(XZ)
	static inline const float m_VELOCITY_MAX_Y = 0.5f;						// �ő�ړ����x(Y)
	static inline const float m_MOVE_MAGNITUDE_INTERPOLATION_SPEED = 0.1f;	// �ړ����x�̖ڕW�l�ւ̕�ԑ��x
	static inline const float m_JUMP_MAGNITUDE = 0.2f;						// �W�����v��
	static inline const float m_GRAVITY = 0.5f;								// �d��
	static inline const float m_ROTATE_INTERPOLATION_SPEED = 0.1f;

	EnemyManager* m_Manager = NULL;
	EnemyCommonParameter* m_CommonParam = NULL;

public:
	EnemyStateBase(EnemyManager* Manager)
		: m_Manager(Manager),
		m_CommonParam(Manager->GetCommonParameter())
	{}
	virtual ~EnemyStateBase() {}

	virtual void Update() = 0;
	virtual void LateUpdate() = 0;

protected:
	// �G�l�~�[����n���\�b�h
	void UpdateMove();
	void CalcMoveSpeed();
	void CalcMoveVelocity();
	void CheckNeedJump();
	void AddGravity();			// �d��
	void CalcResistance();		// ��R
	void CorrectionVelocity();	// �����x�␳
	void RotateAngle();			// �����␳

	void AddVelocity();			// ���W�ɉ����x�𔽉f

	void Jump();

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	virtual int GetStateId() = 0;
	/////////////////////////////
};