/*******************************************************************************
*
*	�^�C�g���F	���܂��G�l�~�[�ڋ߃X�e�[�g�N���X
*	�t�@�C���F	eggEnemyState_Approach.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "eggEnemyState_Approach.h"
#include "manager.h"
#include "gameObject.h"

EggEnemyState_Approach::EggEnemyState_Approach(EggEnemyManager* Manager)
	: EggEnemyStateBase(Manager, EggEnemyManager::AiStateId::Approach)
{
	m_CommonParam->m_MoveMagnitudeMax = m_MOVE_MAGNITUDE_MAX;
}

void EggEnemyState_Approach::Update()
{
	CheckDistance();
	UpdateAngle();
	UpdateMove();
	CheckNeedJump();
	AddGravity();
	CalcResistance();
	CorrectionVelocity();
	RotateAngle();
}

void EggEnemyState_Approach::LateUpdate()
{
	AddVelocity();
}

void EggEnemyState_Approach::CheckDistance()
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
	else if (distanceSq <= m_ATTACK_START_RANGE2 * m_ATTACK_START_RANGE2)
	{// �������߂��ꍇ
		// �U���X�e�[�g�ɑJ��
		m_Manager->RequestNextAiState((int)EggEnemyManager::AiStateId::Attack);
	}
}

void EggEnemyState_Approach::UpdateAngle()
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
