/*******************************************************************************
*
*	�^�C�g���F	�G�l�~�[�������X�N���v�g
*	�t�@�C���F	enemyManager.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"
#include "aabbCollider3d.h"
#include "obbCollider3d.h"
#include "gameObject.h"

class EnemyStateBase;
class EnemyAnimationStateBase;
template<class T>
class CollisionContactRecorder;

class EnemyCommonParameter
{
public:
	// ���ݒn
	float			m_WinceValue = 0.0f;				// ���ݒl
	D3DXVECTOR3		m_Velocity = { 0.0f, 0.0f, 0.0f };	// �����x
	D3DXVECTOR3		m_MoveAngle = { 0.0f, 0.0f, 0.0f };	// �ړ�����
	float			m_MoveMagnitude = 0.0f;				// ���݂̈ړ����x(�}�O�j�`���[�h)
	float			m_MoveAngleDistance = 0.0f;			// �I�u�W�F�N�g�̌����ƈړ������Ƃ̊p�x��
	float			m_NowFrameGravity = 0.0f;			// ���̃t���[���ɂ������Ă���d��
	D3DXVECTOR3		m_SearchRange = { 0.0f, 0.0f, 0.0f };	// �T�m�͈�

	// �ő�l
	float			m_MoveMagnitudeMax = 0.0f;

	EnemyCommonParameter() {}
};

class EnemyManager : public NotDrawComponent
{
public:
	enum class GeneralStateId
	{
		Idle = 0,
		Run,
		Float,
		Jump,
	};

protected:
	// �X�e�[�g�֌W
	EnemyStateBase* m_AiState = NULL;
	EnemyAnimationStateBase* m_AnimationState = NULL;
	GeneralStateId m_GeneralStateId = GeneralStateId::Idle;	// �ėp�X�e�[�gID

	EnemyCommonParameter* m_CommonParameter = NULL;

	// �A�g�R���|�[�l���g
	CollisionContactRecorder<AABBCollider3D>* m_GroundCollision = NULL;
	CollisionContactRecorder<OBBCollider3D>* m_SearchCollision = NULL;
	CollisionContactRecorder<OBBCollider3D>* m_AttackCollision = NULL;
	CollisionContactRecorder<OBBCollider3D>* m_JumpCollision = NULL;

public:
	EnemyManager(GameObject* AttachObject)
		: NotDrawComponent(AttachObject)
	{}
	virtual ~EnemyManager() {}

	virtual void LateInit() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;

	virtual void ChangeNextGeneralState() = 0;				// �ėp�X�e�[�g�̑J�ڏ���
	virtual void RequestNextAiState(int Next) = 0;			// AI�X�e�[�g�̑J�ڃ��N�G�X�g
	virtual void ChangeNextAiState() = 0;					// AI�X�e�[�g�̑J�ڏ���
	virtual void RequestNextAnimationState(int Next) = 0;	// �A�j���[�V�����X�e�[�g�̑J�ڃ��N�G�X�g
	virtual void ChangeNextAnimationState() = 0;			// �A�j���[�V�����X�e�[�g�̑J�ڏ���

	virtual void ShortRollDamage() = 0;
	virtual void RollDamage(float DistanceRate) = 0;
	virtual void LongRollDamage() = 0;

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	GeneralStateId GetGeneralStateId() { return m_GeneralStateId; }
	virtual int GetRecvAiStateId() = 0;
	EnemyCommonParameter* GetCommonParameter() { return m_CommonParameter; }
	CollisionContactRecorder<AABBCollider3D>* GetGroundCollision() { return m_GroundCollision; }
	CollisionContactRecorder<OBBCollider3D>* GetSearchCollision() { return m_SearchCollision; }
	CollisionContactRecorder<OBBCollider3D>* GetAttackCollision() { return m_AttackCollision; }
	CollisionContactRecorder<OBBCollider3D>* GetJumpCollision() { return m_JumpCollision; }
	
	GameObject* GetModelRoot() { return m_AttachObject->GetChild(4); }
	/////////////////////////////
};
