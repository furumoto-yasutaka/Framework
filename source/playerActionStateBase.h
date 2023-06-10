/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���C���X�e�[�g���N���X
*	�t�@�C���F	playerActionStateBase.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "playerManager.h"
#include "playerInputListener.h"

class PlayerActionStateBase
{
protected:
	static inline const float		m_VELOCITY_MAX_XZ = 1.0f;						// �ő�ړ����x(XZ)
	static inline const float		m_VELOCITY_MAX_Y = 0.5f;						// �ő�ړ����x(Y)
	static inline const float		m_WALK_MAGNITUDE = 0.2f;						// �ʏ�ړ��̍ő呬�x
	static inline const float		m_MOVE_MAGNITUDE_INTERPOLATION_SPEED = 0.1f;	// �ړ����x�̖ڕW�l�ւ̕�ԑ��x
	static inline const float		m_TURN_SPEED = 0.2f;							// �U��������x
	static inline const float		m_JUMP_MAGNITUDE = 0.2f;						// �W�����v��
	static inline const float		m_GRAVITY = 0.5f;								// �d��
	static inline const D3DXVECTOR3 m_RESISTANCE = { 0.9f, 0.999f, 0.9f };			// ��R�W��
	static inline const float		m_ROTATE_INTERPOLATION_SPEED = 0.2f;			// �I�u�W�F�N�g������ԑ��x
	static inline const float		m_TURN_OCCUR_ANGLE_THRESHOLD = Math::m_PI<float> * 0.51f;	// �U������𔭐�������

	PlayerManager* m_Manager = NULL;
	PlayerParameter* m_Param = NULL;
	PlayerInputListener* m_InputListener = NULL;
	PlayerManager::ActionStateId m_ActionStateId;

	optional<float> m_CoolTimeCount;	// ���͂̎󂯕t���҂�����
	
	// �e���͂ɑ΂�����s���\�b�h�[�߂��z��
	void(PlayerActionStateBase::* m_ExecMethod[(int)PlayerInputListener::ActionId::Length])(PlayerInputListener::ActionElement* Elem) =
	{
		&PlayerActionStateBase::ExecMove_Z,
		&PlayerActionStateBase::ExecMove_X,
		&PlayerActionStateBase::ExecJump,
		&PlayerActionStateBase::ExecAvoid,
		&PlayerActionStateBase::ExecRoll_Left,
		&PlayerActionStateBase::ExecRoll_Right,
		&PlayerActionStateBase::ExecWeaponDeformation,
	};

public:
	PlayerActionStateBase(PlayerManager* Manager, PlayerManager::ActionStateId ActionStateId)
		: m_Manager(Manager), 
		m_Param(Manager->GetParameter()),
		m_InputListener(Manager->GetInputListener()),
		m_ActionStateId(ActionStateId)
	{
		m_Param->m_MoveMagnitudeMax = m_WALK_MAGNITUDE;
		m_Param->m_TurnSpeed = m_TURN_SPEED;
		m_Param->m_JumpMagnitude = m_JUMP_MAGNITUDE;
	}
	~PlayerActionStateBase() {}

	virtual void Update() = 0;
	virtual void LateUpdate() = 0;

protected:
	virtual void CheckQueue(int Tag);						// �L���[���m�F���ĕK�v�ɉ����ď������s��
	virtual bool CheckElemContinue(bool IsCanExec, int Id);	// �A�N�V���������s���邩�𔻒f
	void UpdateCoolTime();									// �A�N�V�������Ƃ̃N�[���^�C�����X�V

	// �e���͎��s���\�b�h
	virtual void ExecMove_Z(PlayerInputListener::ActionElement* Elem);
	virtual void ExecMove_X(PlayerInputListener::ActionElement* Elem);
	virtual void ExecJump(PlayerInputListener::ActionElement* Elem);
	virtual void ExecAvoid(PlayerInputListener::ActionElement* Elem);
	virtual void ExecRoll_Left(PlayerInputListener::ActionElement* Elem);
	virtual void ExecRoll_Right(PlayerInputListener::ActionElement* Elem);
	virtual void ExecWeaponDeformation(PlayerInputListener::ActionElement* Elem);

	// �v���C���[����n���\�b�h
	void SetMoveInput_Z(float Value);
	void SetMoveInput_X(float Value);
	void UpdateMove();
	float CalcStickTilt(float InputLenSq);
	void CalcMoveSpeed(float TargetMagnitude);
	void CalcMoveVelocity();

	void Jump();

	void AddGravity();			// �d��
	void CalcResistance();		// ��R
	void CorrectionVelocity();	// �����x�␳
	void RotateAngle();			// �����␳
	void AddVelocity();			// ���W�ɉ����x�𔽉f
	void CheckTurnOccur();		// �U������𔭐������邩���f

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	PlayerManager::ActionStateId GetStateId() { return m_ActionStateId; }
	/////////////////////////////
};
