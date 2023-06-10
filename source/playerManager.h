/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�����X�N���v�g
*	�t�@�C���F	playerManager.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"
#include "aabbCollider3d.h"

#include <list>
#include <optional>
#include <string>

class PlayerActionStateBase;
template<class T>
class CollisionContactRecorder_PlayerGround;
template<class T>
class CollisionContactRecorder_PlayerAttack;
class PlayerInputListener;
class PlayerAnimationStateBase;

class PlayerParameter
{
public:
	// ���ݒl
	int				m_Hp = 5;							// ����HP
	int				m_HpMax = 5;						// HP�ő�l
	float			m_InvincibleTime = 0.0f;			// �c�薳�G����
	bool			m_IsGun = false;					// �e��Ԃ�
	bool			m_IsCanAirAvoid = true;				// �󒆂ł̉�����ł����Ԃ�
	D3DXVECTOR3		m_Velocity = { 0.0f, 0.0f, 0.0f };	// �����x

	float			m_MoveMagnitude = 0.0f;				// ���݂̈ړ����x(�}�O�j�`���[�h)
	D3DXVECTOR3		m_MoveInput = { 0.0f, 0.0f, 0.0f };	// �ړ��̓��͕���
	float			m_MoveAngleDistance = 0.0f;			// �I�u�W�F�N�g�̌����Ɠ��͕����Ƃ̊p�x��
	float			m_JumpMagnitude = 0.0f;				// ���݂̃W�����v��(�}�O�j�`���[�h)
	float			m_TurnSpeed = 0.0f;					// �U��������x
	float			m_NowFrameGravity = 0.0f;			// ���̃t���[���ɂ������Ă���d��
	D3DXQUATERNION	m_NowFrameRotateAngle = Math::GetQuaternionIdentity();	// ���̃t���[���ɐU���������

	// �ő�l
	float			m_MoveMagnitudeMax = 0.0f;			// ���݂̈ړ��̍ő呬�x

	PlayerParameter() {}
};

class PlayerManager : public NotDrawComponent
{
public:
	enum class InputModeId
	{
		MouseKeyboard = 0,
		Controller,
	};
	enum class GeneralStateId
	{
		Idle = 0,
		Run,
		Float,
		Jump,
	};
	enum class ActionStateId
	{
		Null = 0,
		None,
		Dash,
		Turn,
		Avoid,
		Roll_Left,
		Roll_Right,
		WeaponDeformation,
	};
	enum class AnimationStateId
	{
		Null = 0,
		None_Gun,
		None_Rod,
		Roll_Left,
		Roll_Right,
		WeaponDeformation_Gun,
		WeaponDeformation_Rod,
	};
	enum class ModelPartsId
	{
		Sphere = 0,				// ���f���L
		Cover,					// ���f���L
		WeaponConnection,		// ���f���L
		WeaponCenter,			// ���f������
		Weapon,					// ���f���L
		Length,
	};

private:
	// �X�e�[�g�֌W
	GeneralStateId				m_GeneralStateId = GeneralStateId::Idle;	// �ėp�X�e�[�gID
	PlayerActionStateBase*		m_ActionState = NULL;						// �A�N�V�����X�e�[�g
	ActionStateId				m_RecvActionStateId = ActionStateId::Null;	// 1�O�̃A�N�V�����X�e�[�g
	ActionStateId				m_NextActionStateId = ActionStateId::Null;	// ���̃A�N�V�����X�e�[�gID
	PlayerAnimationStateBase*	m_AnimationState = NULL;
	AnimationStateId			m_NextAnimationState = AnimationStateId::Null;

	// �I�u�W�F�N�g
	GameObject*				m_StickEnemyList = NULL;
	GameObject*				m_ModelParts[(int)ModelPartsId::Length] = {};

	// �A�g�N���X
	PlayerParameter*		m_Parameter = NULL;			// �p�����[�^
	PlayerInputListener*	m_InputListener = NULL;		// ���͐���

	// �R���|�[�l���g
	CollisionContactRecorder_PlayerGround<AABBCollider3D>*	m_GroundCollision = NULL;
	CollisionContactRecorder_PlayerAttack<OBBCollider3D>*	m_AttackCollision = NULL;

public:
	PlayerManager(GameObject* AttachObject)
		: NotDrawComponent(AttachObject)
	{}
	virtual ~PlayerManager() {}

	void Init(InputModeId InputMode);
	void LateInit() override;
	void Uninit() override;
	void Update() override;
	void LateUpdate() override;

	void RequestNextActionState(ActionStateId Next);		// �A�N�V�����X�e�[�g�̑J�ڃ��N�G�X�g
	void RequestNextAnimationState(AnimationStateId Next);	// �A�j���[�V�����X�e�[�g�̑J�ڃ��N�G�X�g

	void Damage(int Value);

private:
	void ChangeNextGeneralState();							// �ėp�X�e�[�g�̑J�ڏ���
	void ChangeNextActionState();							// �A�N�V�����X�e�[�g�̑J�ڏ���
	void ChangeNextAnimationState();						// �A�j���[�V�����X�e�[�g�̑J�ڏ���

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	PlayerActionStateBase* GetActionState() { return m_ActionState; }
	PlayerAnimationStateBase* GetAnimationState() { return m_AnimationState; }
	ActionStateId GetRecvActionStateId() { return m_RecvActionStateId; }
	GameObject* GetStickEnemyList() { return m_StickEnemyList; }
	GameObject* GetModelParts(ModelPartsId Id) { return m_ModelParts[(int)Id]; }
	PlayerParameter* GetParameter() { return m_Parameter; }
	CollisionContactRecorder_PlayerGround<AABBCollider3D>* GetGroundCollision() { return m_GroundCollision; }
	CollisionContactRecorder_PlayerAttack<OBBCollider3D>* GetAttackCollision() { return m_AttackCollision; }
	PlayerInputListener* GetInputListener() { return m_InputListener; }
	/////////////////////////////

#ifdef _DEBUG
private:
	bool m_IsStop = false;

public:
	void DrawInspector() override;
#endif
};
