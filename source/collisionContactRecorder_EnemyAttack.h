/*******************************************************************************
*
*	�^�C�g���F	�G�l�~�[�U������Ǘ��X�N���v�g
*	�t�@�C���F	collisionContactRecorder_EnemyAttack.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "collisionContactRecorder.h"
#include "playerManager.h"
#include "enemyManager.h"

template<class T>
class CollisionContactRecorder_EnemyAttack : public CollisionContactRecorder<T>
{
private:
	EnemyManager* m_Manager = NULL;
	
	int m_Damage = 0;

	bool m_IsAttacked = false;

public:
	CollisionContactRecorder_EnemyAttack(GameObject* AttachObject)
		: CollisionContactRecorder<T>(AttachObject),
		m_Damage(0)
	{}
	virtual ~CollisionContactRecorder_EnemyAttack() {}

	void LateInit() override
	{
		CollisionContactRecorder<T>::LateInit();

		// ���̉�����܂ł͐e�N���X�̌^���s���ĂȂ��߂�
		// �r���h���̖��O�������ł����G���[�ɂȂ�̂�
		// this��t���Ė��O���������̉����ɍs���悤�ɂ���
		m_Manager = this->m_AttachObject->GetRoot()->GetComponent<EnemyManager>();
	}

	// �Փ˂����I�u�W�F�N�g���L�^(�R�[���o�b�N)
	void ContactEnter(Collider3D* Collider) override
	{
		CollisionContactRecorder<T>::ContactEnter(Collider);

		if (this->m_IsMatchTag)
		{
			if (!m_IsAttacked)
			{
				PlayerManager* player = Collider->GetAttachObject()->GetComponent<PlayerManager>();
				player->Damage(m_Damage);

				m_IsAttacked = true;
			}
		}
	}

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	void SetManager(PlayerManager* Manager) { m_Manager = Manager; }
	/////////////////////////////
};
