/*******************************************************************************
*
*	�^�C�g���F	�v���C���[���C���X�e�[�g���N���X
*	�t�@�C���F	playerActionStateBase.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "playerActionStateBase.h"
#include "collisionContactRecorder_PlayerGround.h"
#include "gameObject.h"
#include "camera.h"

void PlayerActionStateBase::CheckQueue(int Tag)
{
	bool isCanExec = true;

	// �L���[���m�F���ď��������s����
	for (PlayerInputListener::ActionElement* elem : m_InputListener->GetQueue())
	{
		int id = (int)elem->Id;

		if (!CheckElemContinue(isCanExec, id) &&
			Tag & PlayerInputListener::m_ACTION_INFO[id].Tag)
		{
			if (isCanExec)
			{
				isCanExec = PlayerInputListener::m_ACTION_INFO[id].IsCanOverLap;
			}

			// ���͂ɉ��������������s
			(this->*m_ExecMethod[id])(elem);
			// ���͂��󗝂����̂ŗv�f�͍폜
			m_InputListener->DeleteRequest(elem);
		}
	}
}
bool PlayerActionStateBase::CheckElemContinue(bool IsCanExec, int Id)
{
	// ���ɓ����󗝕s�̓��͂��󗝂��� ���� �����󗝕s��
	// �܂���
	// �Y���̃A�N�V�����̃N�[���^�C�����c���Ă���ꍇ
	if ((!IsCanExec && !PlayerInputListener::m_ACTION_INFO[Id].IsCanOverLap) ||
		m_InputListener->GetActionCoolTime(Id))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void PlayerActionStateBase::UpdateCoolTime()
{
	if (m_CoolTimeCount)
	{
		m_CoolTimeCount.value() -= Time::GetDeltaTimeSec();
		if (m_CoolTimeCount <= 0.0f)
		{
			m_CoolTimeCount.reset();
		}
	}
}

/*******************************************************************************
*	�e���͎��s���\�b�h
*******************************************************************************/
void PlayerActionStateBase::ExecMove_Z(PlayerInputListener::ActionElement* Elem)
{
	SetMoveInput_Z(Elem->Value);
}
void PlayerActionStateBase::ExecMove_X(PlayerInputListener::ActionElement* Elem)
{
	SetMoveInput_X(Elem->Value);
}
void PlayerActionStateBase::ExecJump(PlayerInputListener::ActionElement* Elem)
{
	Jump();
}
void PlayerActionStateBase::ExecAvoid(PlayerInputListener::ActionElement* Elem)
{
	if (m_Manager->GetGroundCollision()->GetIsContact() ||
		m_Param->m_IsCanAirAvoid)
	{
		m_Manager->RequestNextActionState(PlayerManager::ActionStateId::Avoid);
	}
}
void PlayerActionStateBase::ExecRoll_Left(PlayerInputListener::ActionElement* Elem)
{
	m_Manager->RequestNextActionState(PlayerManager::ActionStateId::Roll_Left);
}
void PlayerActionStateBase::ExecRoll_Right(PlayerInputListener::ActionElement* Elem)
{
	m_Manager->RequestNextActionState(PlayerManager::ActionStateId::Roll_Right);
}
void PlayerActionStateBase::ExecWeaponDeformation(PlayerInputListener::ActionElement* Elem)
{
	m_Manager->RequestNextActionState(PlayerManager::ActionStateId::WeaponDeformation);
}

/*******************************************************************************
*	�v���C���[����n���\�b�h
*******************************************************************************/
void PlayerActionStateBase::SetMoveInput_Z(float Value)
{
	m_Param->m_MoveInput.z = Value;
}
void PlayerActionStateBase::SetMoveInput_X(float Value)
{
	m_Param->m_MoveInput.x = Value;
}
void PlayerActionStateBase::UpdateMove()
{
	float lenSq = D3DXVec3LengthSq(&m_Param->m_MoveInput);
	float targetMoveMagnitude;

	if (lenSq != 0.0f)
	{// ���͂���
		// �X�e�B�b�N�̌X�����v�Z
		float tilt = CalcStickTilt(lenSq);
		targetMoveMagnitude = tilt * m_Param->m_MoveMagnitudeMax;
	}
	else
	{// ���͂Ȃ�
		targetMoveMagnitude = 0.0f;
	}

	// �X�s�[�h���X�V
	CalcMoveSpeed(targetMoveMagnitude);

	// �����x������
	CalcMoveVelocity();
}
float PlayerActionStateBase::CalcStickTilt(float InputLenSq)
{
	//------------------------------------------------
	// �X�e�B�b�N�̌X�����ψ�ɂȂ�悤���K�����đ��x�����߂�
	//------------------------------------------------
	float angle, halfPi, rate;
	// �x�N�g���̊p�x�����߂�
	angle = atan2f(m_Param->m_MoveInput.z, m_Param->m_MoveInput.x);
	// 90�x(���W�A��)�Ŋ��������܂�����߂�
	halfPi = Math::m_PI<float> * 0.5f;
	while (angle > halfPi) { angle -= halfPi; }
	// �x�N�g���̔{�������߂�(�p�x���΂�(45�x)�ł���قǒl���傫���Ȃ�)
	rate = fabs(sinf(angle * 2)) + 1.0f;
	// �������X�������߂�
	return InputLenSq / rate;
}
void PlayerActionStateBase::CalcMoveSpeed(float TargetMagnitude)
{
	D3DXVECTOR3 notYVel = { m_Param->m_Velocity.x, 0.0f, m_Param->m_Velocity.z };
	float mag = D3DXVec3Length(&notYVel);

	// �����蔻��⏔�X�̉e���Ŏ��ۂ̑��x���ቺ���Ă��܂����ꍇ�͈ړ����x���C������
	if (mag < m_Param->m_MoveMagnitude)
	{
		m_Param->m_MoveMagnitude = mag;
	}

	// ���x��ڕW�l�Ɍ����ĕ␳
	m_Param->m_MoveMagnitude += (TargetMagnitude - m_Param->m_MoveMagnitude) * m_MOVE_MAGNITUDE_INTERPOLATION_SPEED;
}
void PlayerActionStateBase::CalcMoveVelocity()
{
	D3DXVECTOR3 forward = m_Manager->GetAttachObject()->GetForward();
	forward = Math::Normalize(forward.x, 0.0f, forward.z);
	D3DXVECTOR3 vel = Math::Normalize(forward) * m_Param->m_MoveMagnitude;
	vel.y = m_Param->m_Velocity.y;

	m_Param->m_Velocity = vel;
}

void PlayerActionStateBase::Jump()
{
	m_Param->m_Velocity.y = m_Param->m_JumpMagnitude;
}

void PlayerActionStateBase::AddGravity()
{
	m_Param->m_NowFrameGravity = m_GRAVITY * Time::GetDeltaTimeSec();
	m_Param->m_Velocity.y -= m_Param->m_NowFrameGravity;
}
void PlayerActionStateBase::CalcResistance()
{
	// ��R���v�Z
	//m_Param->m_Velocity.x *= m_Param->m_RESISTANCE.x;
	//m_Param->m_Velocity.y *= m_Param->m_RESISTANCE.y;
	//m_Param->m_Velocity.z *= m_Param->m_RESISTANCE.z;
}
void PlayerActionStateBase::CorrectionVelocity()
{
	// ���x�̕␳(XZ)
	D3DXVECTOR3 tempVel = { m_Param->m_Velocity.x, 0.0f, m_Param->m_Velocity.z };
	float len = D3DXVec3LengthSq(&tempVel);
	if (len > m_VELOCITY_MAX_XZ * m_VELOCITY_MAX_XZ)
	{
		float y = m_Param->m_Velocity.y;
		m_Param->m_Velocity = Math::Normalize(tempVel) * m_VELOCITY_MAX_XZ;
		m_Param->m_Velocity.y = y;
	}

	// ���x�̕␳(Y)
	if (m_Param->m_Velocity.y > m_VELOCITY_MAX_Y)
	{
		m_Param->m_Velocity.y = m_VELOCITY_MAX_Y;
	}
	else if (m_Param->m_Velocity.y < -m_VELOCITY_MAX_Y)
	{
		m_Param->m_Velocity.y = -m_VELOCITY_MAX_Y;
	}
}
void PlayerActionStateBase::RotateAngle()
{
	GameObject* obj = Manager::GetScene()->GetActiveCamera()->GetAttachObject();
	D3DXVECTOR3 cameraForward = obj->GetForward();
	D3DXVECTOR3 cameraRight = obj->GetRight();
	cameraForward = { cameraForward.x, 0.0f, cameraForward.z };
	cameraRight = { cameraRight.x, 0.0f, cameraRight.z };
	D3DXVECTOR3 notYVel = cameraForward * m_Param->m_MoveInput.z + cameraRight * m_Param->m_MoveInput.x;

	// ���͂��Ȃ��ꍇ�͕␳���Ȃ�
	if (D3DXVec3LengthSq(&notYVel) <= FLT_EPSILON)
	{
		m_Param->m_NowFrameRotateAngle = Math::GetQuaternionIdentity();
		return;
	}

	D3DXVECTOR3 playerForward = m_Manager->GetAttachObject()->GetForward();
	playerForward = { playerForward.x, 0.0f, playerForward.z };
	playerForward = Math::Normalize(playerForward);
	D3DXQUATERNION q = Math::GetQuaternionIdentity();
	D3DXVECTOR3 normalNotY = Math::Normalize(notYVel);
	D3DXVECTOR3 y = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3 axis;

	// �U������Ɏg����]��������
	if (normalNotY == -playerForward)
	{// z���Ɛ����ȏꍇ
		// �O�ς̌��ʂ����������Ȃ�̂Ŏ蓮�Ŏw��
		axis = y;
	}
	else
	{// z���Ɛ����ł͂Ȃ��ꍇ
		// �O�ς��g�����Ƃŋ߂�����������]�������߂���
		D3DXVec3Cross(&axis, &playerForward, &normalNotY);
	}

	// ���݂̌����ƌ������������Ƃ̂Ȃ��p�����߂�
	m_Param->m_MoveAngleDistance = Math::VectorAngle(normalNotY, playerForward);
	float angle = m_Param->m_MoveAngleDistance * m_ROTATE_INTERPOLATION_SPEED;
	// ��]�N�I�[�^�j�I�������
	q *= Math::AxisRotation_Radian(axis, angle);
	m_Param->m_NowFrameRotateAngle = q;
	// ��]�̔��f
	m_Manager->GetAttachObject()->m_LocalRotation *= q;
}
void PlayerActionStateBase::AddVelocity()
{
	m_Manager->GetAttachObject()->m_LocalPosition += m_Param->m_Velocity;
}
void PlayerActionStateBase::CheckTurnOccur()
{
	if (m_Param->m_MoveAngleDistance >= m_TURN_OCCUR_ANGLE_THRESHOLD)
	{
		m_Manager->RequestNextActionState(PlayerManager::ActionStateId::Turn);
	}
}
