/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���͎�t�X�N���v�g
*	�t�@�C���F	playerInputListener.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"

#include <list>
#include <optional>

class PlayerManager;

class PlayerInputListener
{
public:
	enum class ActionId
	{
		Move_Z = 0,
		Move_X,
		Jump,
		Avoid,
		Roll_Left,
		Roll_Right,
		WeaponDeformation,
		Length,
	};
	struct ActionElement
	{
		ActionId Id;
		float Value;
		float ValidTime;

		ActionElement(ActionId Id, float Value, float ValidTime)
		{
			this->Id = Id;
			this->Value = Value;
			this->ValidTime = ValidTime;
		}
	};
	struct ActionInfo
	{
		bool IsCanOverLap;
		int Tag;
	};
	// �e�A�N�V�����̓��͂ɂ��Ă̏��
	static inline const ActionInfo m_ACTION_INFO[(int)ActionId::Length] =
	{
		{ true,		(1 << (int)ActionId::Move_Z) },
		{ true,		(1 << (int)ActionId::Move_X) },
		{ true,		(1 << (int)ActionId::Jump) },
		{ false,	(1 << (int)ActionId::Avoid) },
		{ false,	(1 << (int)ActionId::Roll_Left) },
		{ false,	(1 << (int)ActionId::Roll_Right) },
		{ false,	(1 << (int)ActionId::WeaponDeformation) },
	};

private:
	static inline const float	m_VALID_TIME = 0.5f;		// ��s���͗L������

	PlayerManager*				m_Manager = NULL;
	list<ActionElement*>		m_ActionQueue;				// ���͏��̃L���[
	list<ActionElement*>		m_DeleteList;				// �폜����\��̃L���[

	optional<float> m_ActionCoolTime[(int)ActionId::Length];

public:
	PlayerInputListener(PlayerManager* Manager) : m_Manager(Manager) {}

	virtual void UpdateQueue();	// �폜�\��̃L���[�̍폜�Ɠ��͂̎擾���s��
	void DeleteRequest(ActionElement* Elem);
	void SetActionCoolTime(ActionId Id, float Time);
	void ResetActionCoolTime(ActionId Id);
	void ResetActionCoolTimeAll();
	void DeleteElementAll();
	
	// ���͎擾
	virtual float InputMove_Z() = 0;			// �ړ�����(�O��)
	virtual float InputMove_X() = 0;			// �ړ�����(���E)
	virtual bool InputJump() = 0;				// �W�����v����
	virtual bool InputAvoid() = 0;				// ������
	virtual optional<float> InputRoll_L() = 0;	// ���͈͉�]����
	virtual optional<float> InputRoll_R() = 0;	// ���͈͉�]����
	virtual bool InputWeaponDeformation() = 0;	// ����ό`����

private:
	void CheckValidTime();	// ���̗͂L�����Ԃ��m�F
	void DeleteElement();	// �L�����Ԑ؂�̗v�f���L���[����폜
	void UpdateCoolTime();	// ���͂��Ƃ̃N�[���^�C�����X�V

	// �L���[�ɒǉ�
	void SetMoveAction();				// �ړ�
	void SetJumpAction();				// �W�����v
	void SetAvoidAction();				// ���
	void SetRollAction();				// ���͈͉�]
	void SetWeaponDeformationAction();	// ����ό`

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	void SetManager(PlayerManager* Manager) { m_Manager = Manager; }

	list<ActionElement*> GetQueue() { return m_ActionQueue; }

	optional<float> GetActionCoolTime(int Id) { return m_ActionCoolTime[Id]; }
	/////////////////////////////
};
