/*******************************************************************************
*
*	�^�C�g���F	�G�l�~�[�X�e�[�g���N���X
*	�t�@�C���F	enemyStateBase.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "enemyStateBase.h"
#include "enemyManager.h"
#include "gameObject.h"
#include "collisionContactRecorder.h"

void EnemyStateBase::UpdateMove()
{
	// �X�s�[�h���X�V
	CalcMoveSpeed();

	// �����x������
	CalcMoveVelocity();
}

void EnemyStateBase::CalcMoveSpeed()
{
	D3DXVECTOR3 notYVel = { m_CommonParam->m_Velocity.x, 0.0f, m_CommonParam->m_Velocity.z };
	float mag = D3DXVec3Length(&notYVel);

	// �����蔻��⏔�X�̉e���Ŏ��ۂ̑��x���ቺ���Ă��܂����ꍇ�͈ړ����x���C������
	if (mag < m_CommonParam->m_MoveMagnitude)
	{
		m_CommonParam->m_MoveMagnitude = mag;
	}

	// ���x��ڕW�l�Ɍ����ĕ␳
	m_CommonParam->m_MoveMagnitude +=
		(m_CommonParam->m_MoveMagnitudeMax - m_CommonParam->m_MoveMagnitude) * m_MOVE_MAGNITUDE_INTERPOLATION_SPEED;
}

void EnemyStateBase::CalcMoveVelocity()
{
	D3DXVECTOR3 forward = m_Manager->GetAttachObject()->GetForward();
	forward = Math::Normalize(forward.x, 0.0f, forward.z);
	D3DXVECTOR3 vel = Math::Normalize(forward) * m_CommonParam->m_MoveMagnitude;
	vel.y = m_CommonParam->m_Velocity.y;

	m_CommonParam->m_Velocity = vel;
}

void EnemyStateBase::CheckNeedJump()
{
	// �n�ʂ��ړ������ǂ��ڋ߂��Ă���ꍇ�W�����v����
	if (m_Manager->GetGeneralStateId() == EggEnemyManager::GeneralStateId::Run &&
		m_Manager->GetJumpCollision()->GetIsContact())
	{
		Jump();
	}
}

void EnemyStateBase::AddGravity()
{
	m_CommonParam->m_NowFrameGravity = m_GRAVITY * Time::GetDeltaTimeSec();
	m_CommonParam->m_Velocity.y -= m_CommonParam->m_NowFrameGravity;
}

void EnemyStateBase::CalcResistance()
{
	// ��R���v�Z
	//m_CommonParam->m_Velocity.x *= m_CommonParam->m_RESISTANCE.x;
	//m_CommonParam->m_Velocity.y *= m_CommonParam->m_RESISTANCE.y;
	//m_CommonParam->m_Velocity.z *= m_CommonParam->m_RESISTANCE.z;
}

void EnemyStateBase::CorrectionVelocity()
{
	// ���x�̕␳(XZ)
	D3DXVECTOR3 tempVel = { m_CommonParam->m_Velocity.x, 0.0f, m_CommonParam->m_Velocity.z };
	float len = D3DXVec3LengthSq(&tempVel);
	if (len > m_VELOCITY_MAX_XZ * m_VELOCITY_MAX_XZ)
	{
		float y = m_CommonParam->m_Velocity.y;
		m_CommonParam->m_Velocity = Math::Normalize(tempVel) * m_VELOCITY_MAX_XZ;
		m_CommonParam->m_Velocity.y = y;
	}

	// ���x�̕␳(Y)
	if (m_CommonParam->m_Velocity.y > m_VELOCITY_MAX_Y)
	{
		m_CommonParam->m_Velocity.y = m_VELOCITY_MAX_Y;
	}
	else if (m_CommonParam->m_Velocity.y < -m_VELOCITY_MAX_Y)
	{
		m_CommonParam->m_Velocity.y = -m_VELOCITY_MAX_Y;
	}
}

void EnemyStateBase::RotateAngle()
{
	D3DXVECTOR3 notYMoveVel = m_CommonParam->m_MoveAngle;

	// �������Ȃ��ꍇ�͕␳���Ȃ�
	if (D3DXVec3LengthSq(&notYMoveVel) <= FLT_EPSILON)
	{
		return;
	}


	D3DXVECTOR3 eggForward = m_Manager->GetAttachObject()->GetForward();
	eggForward = { eggForward.x, 0.0f, eggForward.z };
	eggForward = Math::Normalize(eggForward);

	D3DXQUATERNION q = Math::GetQuaternionIdentity();
	D3DXVECTOR3 normalNotY = Math::Normalize(notYMoveVel);
	D3DXVECTOR3 y = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3 axis;

	// �U������Ɏg����]��������
	if (normalNotY == -eggForward)
	{// z���Ɛ����ȏꍇ
		// �O�ς̌��ʂ����������Ȃ�̂Ŏ蓮�Ŏw��
		axis = y;
	}
	else
	{// z���Ɛ����ł͂Ȃ��ꍇ
		// �O�ς��g�����Ƃŋ߂�����������]�������߂���
		D3DXVec3Cross(&axis, &eggForward, &normalNotY);
	}

	// ���݂̌����ƌ������������Ƃ̂Ȃ��p�����߂�
	m_CommonParam->m_MoveAngleDistance = Math::VectorAngle(normalNotY, eggForward);
	float angle = m_CommonParam->m_MoveAngleDistance * m_ROTATE_INTERPOLATION_SPEED;
	// ��]�N�I�[�^�j�I�������
	q *= Math::AxisRotation_Radian(axis, angle);
	// ��]�̔��f
	m_Manager->GetAttachObject()->m_LocalRotation *= q;
}

void EnemyStateBase::AddVelocity()
{
	m_Manager->GetAttachObject()->m_LocalPosition += m_CommonParam->m_Velocity;
}

void EnemyStateBase::Jump()
{
	m_CommonParam->m_Velocity.y = m_JUMP_MAGNITUDE;
}
