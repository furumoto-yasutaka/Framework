/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�ڒn�Ǘ��X�N���v�g
*	�t�@�C���F	collisionContactRecorder_PlayerAttack.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "collisionContactRecorder.h"
#include "playerManager.h"
#include "enemyManager.h"
#include "playerActionStateBase.h"

#include "eggEnemyManager.h"

template<class T>
class CollisionContactRecorder_PlayerAttack : public CollisionContactRecorder<T>
{
private:
	PlayerManager* m_Manager = NULL;

public:
	CollisionContactRecorder_PlayerAttack(GameObject* AttachObject) :
		CollisionContactRecorder<T>(AttachObject)
	{}
	~CollisionContactRecorder_PlayerAttack() {}

	void LateInit() override
	{
		CollisionContactRecorder<T>::LateInit();

		// ���̉�����܂ł͐e�N���X�̌^���s���ĂȂ��߂�
		// �r���h���̖��O�������ł����G���[�ɂȂ�̂�
		// this��t���Ė��O���������̉����ɍs���悤�ɂ���
		m_Manager = this->m_AttachObject->GetRoot()->GetComponent<PlayerManager>();
	}

	// �Փ˂����I�u�W�F�N�g���L�^(�R�[���o�b�N)
	void ContactEnter(Collider3D* Collider) override
	{
		CollisionContactRecorder<T>::ContactEnter(Collider);

		if (this->m_IsMatchTag)
		{
			// �S�Ă̓G��Manager�̎擾�����݂�
			EnemyManager* enemy = Collider->GetAttachObject()->GetComponent<EggEnemyManager>();
			float DistanceRate = 0.0f;

			DistanceRate = CalcDistanceRate(enemy);
			enemy->RollDamage(DistanceRate);
		}
	}

	float CalcDistanceRate(EnemyManager* Enemy)
	{
		D3DXVECTOR3 playerPos = this->m_AttachObject->GetRoot()->m_LocalPosition;
		D3DXVECTOR3 enemyPos = Enemy->GetAttachObject()->GetWorldPosition();
		D3DXVECTOR3 distanceVec = enemyPos - playerPos;

		return D3DXVec3Length(&distanceVec) * 0.2f;
	}

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	void SetManager(PlayerManager* Manager) { m_Manager = Manager; }
	/////////////////////////////
};
