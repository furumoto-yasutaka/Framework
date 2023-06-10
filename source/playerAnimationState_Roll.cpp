/*******************************************************************************
*
*	�^�C�g���F	�v���C���[�A�j���[�V�������͈͉�]�X�e�[�g�N���X
*	�t�@�C���F	playerAnimationState_Roll.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerAnimationState_Roll.h"
#include "playerManager.h"
#include "gameObject.h"
#include "camera.h"
#include "collisionContactRecorder_PlayerAttack.h"

void PlayerAnimationState_Roll::MainRodRotate(D3DXVECTOR3 Axis)
{
	// ������ԂŖ_�����������
	float amount;
	if (m_ROTATE_ANGLE - m_RotatedAngle <= m_ROTATE_THRESHOLD)
	{
		m_IsEndRotate = true;
	}
	amount = (m_ROTATE_INTERPOLATION_ANGLE_REF - m_RotatedAngle) * m_ROTATE_INTERPOLATION_RATE;
	m_RotatedAngle += amount;

	GameObject* rodConnection = m_Manager->GetModelParts(PlayerManager::ModelPartsId::WeaponConnection);
	rodConnection->m_LocalRotation *= Math::AxisRotation_Degree(Axis, amount);
}

void PlayerAnimationState_Roll::PreEndRodRotate()
{
	// ���݂̖_�̈ʒu���猳�̖_�̈ʒu�܂Ŋ�����Ԃňړ�����
	D3DXVECTOR3 cameraForward = Manager::GetScene()->GetActiveCamera()->GetAttachObject()->GetForward();
	cameraForward = { cameraForward.x, 0.0f, cameraForward.z };
	cameraForward = Math::Normalize(cameraForward);

	GameObject* weaponConnection = m_Manager->GetModelParts(PlayerManager::ModelPartsId::WeaponConnection);
	D3DXVECTOR3 weaponForward = weaponConnection->GetForward();
	weaponForward = { weaponForward.x, 0.0f, weaponForward.z };
	weaponForward = Math::Normalize(weaponForward);

	D3DXVECTOR3 y = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3 axis;
	D3DXQUATERNION q = Math::GetQuaternionIdentity();

	// �U������Ɏg����]��������
	if (weaponForward == -cameraForward)
	{// z���Ɛ����ȏꍇ
		// �O�ς̌��ʂ����������Ȃ�̂Ŏ蓮�Ŏw��
		axis = y;
	}
	else
	{// z���Ɛ����ł͂Ȃ��ꍇ
		// �O�ς��g�����Ƃŋ߂�����������]�������߂���
		D3DXVec3Cross(&axis, &weaponForward, &cameraForward);
	}

	// ���݂̌����ƌ������������Ƃ̂Ȃ��p�����߂�
	float angle = Math::VectorAngle(weaponForward, cameraForward);
	if (angle > m_END_ROTATE_THRESHOLD)
	{
		angle *= m_END_ROTATE_INTERPOLATION_RATE;
	}
	else
	{
		m_IsEndRotate = true;
	}
	// ��]�N�I�[�^�j�I�������
	q *= Math::AxisRotation_Radian(axis, angle);
	// ��]�̔��f
	weaponConnection->m_LocalRotation *= q;
}

void PlayerAnimationState_Roll::MainScaleChange()
{
	//// �_�̃X�P�[����ω�������
	//GameObject* obj = m_Manager->GetAttackCollision()->GetAttachObject()->GetParent();
	//float amount = (1.0f - obj->m_LocalScale.x);

	//if (amount > m_SCALE_INTERPOLATION_THRESHOLD)
	//{
	//	amount *= m_SCALE_INTERPOLATION_RATE;
	//}
	//else
	//{
	//	m_IsEndScaling = true;
	//}

	//obj->m_LocalScale += { amount, amount, amount };

	m_IsEndScaling = true;
}

void PlayerAnimationState_Roll::PreEndScaleChange()
{
	//// �_�̃X�P�[����ω�������
	//GameObject* obj = m_Manager->GetAttackCollision()->GetAttachObject()->GetParent();
	//float amount = obj->m_LocalScale.x;

	//if (amount > m_SCALE_INTERPOLATION_THRESHOLD)
	//{
	//	amount *= m_SCALE_INTERPOLATION_RATE;
	//}
	//else
	//{
	//	m_IsEndScaling = true;
	//}

	//obj->m_LocalScale -= { amount, amount, amount };

	m_IsEndScaling = true;
}
