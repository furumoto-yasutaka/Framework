/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�����X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_Find.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "eggEnemyState_Find.h"
#include "manager.h"
#include "gameObject.h"

EggEnemyState_Find::EggEnemyState_Find(EggEnemyManager* Manager)
	: EggEnemyStateBase(Manager, EggEnemyManager::AiStateId::Find)
{
	m_Manager->RequestNextAnimationState((int)EggEnemyManager::AnimationStateId::Find);
}

void EggEnemyState_Find::Update()
{
	m_WaitTimeLimit -= Time::GetDeltaTimeSec();
	if (m_WaitTimeLimit <= 0.0f)
	{
		// �ǐՃX�e�[�g�ɑJ��
		m_Manager->RequestNextAiState((int)EggEnemyManager::AiStateId::Approach);
	}

	CheckDistance();
	UpdateAngle();
	UpdateMove();
	AddGravity();
	CalcResistance();
	CorrectionVelocity();
	RotateAngle();
}

void EggEnemyState_Find::LateUpdate()
{
	AddVelocity();
}

void EggEnemyState_Find::CheckDistance()
{
	// ���E�Ƀv���C���[�����݂��邩���f
	// �v���C���[���T�m�p�{�b�N�X�͈̔͂ɓ������猩�����Ƃ���


	// �v���C���[�Ƃ̊Ԃɏ�Q�������邩���f
	// ���C���v���C���[�̒����Ɍ����Ĕ��˂���


	//----------------------------
	// �Ƃ肠�������ł̓v���C���[�ƈ��̋����ȉ��ɂȂ����猩�����Ƃ���
	GameObject* player = Manager::GetScene()->GetGameObject("Player");
	D3DXVECTOR3 eggPos = m_Manager->GetAttachObject()->GetWorldPosition();
	D3DXVECTOR3 playerPos = player->GetWorldPosition();
	D3DXVECTOR3 distanceVec = playerPos - eggPos;
	float distanceSq = D3DXVec3LengthSq(&distanceVec);

	if (distanceSq > m_SEARCH_RANGE2 * m_SEARCH_RANGE2)
	{// �����������Ȃ����ꍇ
		// �p�j�X�e�[�g�ɑJ��
		m_Manager->RequestNextAiState((int)EggEnemyManager::AiStateId::Search);
	}
}

void EggEnemyState_Find::UpdateAngle()
{
	GameObject* player = Manager::GetScene()->GetGameObject("Player");
	D3DXVECTOR3 eggPos = m_Manager->GetAttachObject()->GetWorldPosition();
	D3DXVECTOR3 playerPos = player->GetWorldPosition();
	D3DXVECTOR3 distanceVec = playerPos - eggPos;
	distanceVec = { distanceVec.x, 0.0f, distanceVec.z };
	D3DXVECTOR3 distanceVecNormal = Math::Normalize(distanceVec);
	float distanceSq = D3DXVec3LengthSq(&distanceVec);

	if (distanceSq <= 0.0f)
	{// �x�N�g�����s���ȏꍇ
		// �p�j�X�e�[�g�ɑJ�ڂ���
		m_Manager->RequestNextAiState((int)EggEnemyManager::AiStateId::Search);
	}
	else
	{// �x�N�g��������ȏꍇ
		// �ړ��������X�V����
		m_CommonParam->m_MoveAngle = distanceVecNormal;
	}
}
