/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�ڒn�Ǘ��X�N���v�g
*	�t�@�C���F	collisionContactRecorder_PlayerGround.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "collisionContactRecorder.h"
#include "playerManager.h"

template<class T>
class CollisionContactRecorder_PlayerGround : public CollisionContactRecorder<T>
{
private:
	PlayerManager* m_Manager = NULL;

public:
	CollisionContactRecorder_PlayerGround(GameObject* AttachObject) :
		CollisionContactRecorder<T>(AttachObject)
	{}
	~CollisionContactRecorder_PlayerGround() {}

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

		if (this->m_IsContact)
		{
			m_Manager->GetParameter()->m_IsCanAirAvoid = true;
		}
	}

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	void SetManager(PlayerManager* Manager) { m_Manager = Manager; }
	/////////////////////////////
};
