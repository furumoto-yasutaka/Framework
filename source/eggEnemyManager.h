/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�����X�N���v�g
*	�t�@�C���F	eggEnemyManager.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "enemyManager.h"

class EggEnemyManager : public EnemyManager
{
public:
	enum class AiStateId
	{
		Null = 0,		// ����
		Search,			// �p�j
		Approach,		// �ڋ�
		Find,			// ����
		Attack,			// �U��
		KnockBack,		// �m�b�N�o�b�N
		Wince,			// �Ђ��
		Stick,			// �����t��
		Death,			// ���S
	};
	enum class AnimationStateId
	{
		Null = 0,
		None,
		Find,
		Approach,
		HeadAttack,
		KnockBack,
		Wince,
	};

private:
	static inline const float m_WINCE_VALUE_MAX = 3.0f;
	static inline const float m_ROLL_DISTANCE_RATE_STICK_THRESHOLD = 0.7f;

	AiStateId m_RecvAiStateId = AiStateId::Null;
	AiStateId m_NextAiStateId = AiStateId::Null;

	AnimationStateId m_NextAnimationStateId = AnimationStateId::Null;

public:
	EggEnemyManager(GameObject* AttachObject)
		: EnemyManager(AttachObject)
	{}
	~EggEnemyManager() {}

	void Init();
	void LateInit() override;
	void Uninit() override;
	void Update() override;
	void LateUpdate() override;

	void ChangeNextGeneralState() override;				// �ėp�X�e�[�g�̑J�ڏ���
	void RequestNextAiState(int Next) override;			// AI�X�e�[�g�̑J�ڃ��N�G�X�g
	void ChangeNextAiState() override;					// AI�X�e�[�g�̑J�ڏ���
	void RequestNextAnimationState(int Next) override;	// �A�j���[�V�����X�e�[�g�̑J�ڃ��N�G�X�g
	void ChangeNextAnimationState() override;			// �A�j���[�V�����X�e�[�g�̑J�ڏ���

	void ShortRollDamage() override;
	void RollDamage(float DistanceRate) override;
	void LongRollDamage() override;

	void ChangeStickState();

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	int GetRecvAiStateId() override { return (int)m_RecvAiStateId; }
	AnimationStateId GetNextAnimationStateId() { return m_NextAnimationStateId; }
	/////////////////////////////

#ifdef _DEBUG
private:
	bool m_IsStop = false;

public:
	void DrawInspector() override;
#endif
};
