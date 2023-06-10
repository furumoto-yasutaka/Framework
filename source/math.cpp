/*******************************************************************************
*
*	�^�C�g���F	���w�I�֗��֐��p�ÓI�N���X
*	�t�@�C���F	math.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "math.h"

#include <cmath>

/*******************************************************************************
*	�x�N�g���̐��K��(D3DXVECTOR3)
*******************************************************************************/
D3DXVECTOR3 Math::Normalize(D3DXVECTOR3 vector)
{
	float magnitude = std::sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	float ratio = 1 / magnitude;
	D3DXVECTOR3 result = vector;

	result *= ratio;

	return result;
}
/*******************************************************************************
*	�x�N�g���̐��K��(float3��)
*******************************************************************************/
D3DXVECTOR3 Math::Normalize(float x, float y, float z)
{
	float magnitude = std::sqrtf(x * x + y * y + z * z);
	float ratio = 1 / magnitude;
	D3DXVECTOR3 result = D3DXVECTOR3(x, y, z);

	result *= ratio;
	return result;
}
/*******************************************************************************
*	�x�N�g���̐��K��(D3DXVECTOR2)
*******************************************************************************/
D3DXVECTOR2 Math::Normalize(D3DXVECTOR2 vector)
{
	float magnitude = std::sqrtf(vector.x * vector.x + vector.y * vector.y);
	float ratio = 1 / magnitude;
	D3DXVECTOR2 result = vector;

	result *= ratio;
	return result;
}
/*******************************************************************************
*	�x�N�g���̐��K��(float2��)
*******************************************************************************/
D3DXVECTOR2 Math::Normalize(float x, float y)
{
	float magnitude = std::sqrtf(x * x + y * y);
	float ratio = 1 / magnitude;
	D3DXVECTOR2 result = D3DXVECTOR2(x, y);

	result *= ratio;
	return result;
}

/*******************************************************************************
*	��������(int)
*******************************************************************************/
int Math::Random(int max)
{
	return rand() % max;
}
/*******************************************************************************
*	��������(float)
*******************************************************************************/
float Math::Random(float max)
{
	return (float)rand() / RAND_MAX * max;
}
/*******************************************************************************
*	��������(D3DXVECTOR2)
*******************************************************************************/
D3DXVECTOR2 Math::Random(D3DXVECTOR2 max)
{
	D3DXVECTOR2 value;
	value.x = (float)rand() / RAND_MAX * max.x;
	value.y = (float)rand() / RAND_MAX * max.y;
	return value;
}
/*******************************************************************************
*	��������(D3DXVECTOR3)
*******************************************************************************/
D3DXVECTOR3 Math::Random(D3DXVECTOR3 max)
{
	D3DXVECTOR3 value;
	value.x = (float)rand() / RAND_MAX * max.x;
	value.y = (float)rand() / RAND_MAX * max.y;
	value.z = (float)rand() / RAND_MAX * max.z;
	return value;
}

/*******************************************************************************
*	�x���@���烉�W�A���ɕϊ�(1��)
*******************************************************************************/
float Math::DegreeToRadian(float degree)
{
	return degree * m_PI_DIV_HALF<float>;
}
/*******************************************************************************
*	�x���@���烉�W�A���ɕϊ�(3��)
*******************************************************************************/
D3DXVECTOR3 Math::DegreeToRadian(D3DXVECTOR3 degree)
{
	return degree * m_PI_DIV_HALF<float>;
}

/*******************************************************************************
*	���W�A������x���@�ɕϊ�(1��)
*******************************************************************************/
float Math::RadianToDegree(float radian)
{
	return radian * m_HALF_DIV_PI<float>;
}
/*******************************************************************************
*	���W�A������x���@�ɕϊ�(3��)
*******************************************************************************/
D3DXVECTOR3 Math::RadianToDegree(D3DXVECTOR3 radian)
{
	return radian * m_HALF_DIV_PI<float>;
}

/*******************************************************************************
*	�I�C���[�p���N�I�[�^�j�I���ɕϊ�(float�����w��)
*******************************************************************************/
D3DXQUATERNION Math::EulerAngleToQuaternion(float x, float y, float z)
{
	float cosX = cosf(x * 0.5f);
	float sinX = sinf(x * 0.5f);
	float cosY = cosf(y * 0.5f);
	float sinY = sinf(y * 0.5f);
	float cosZ = cosf(z * 0.5f);
	float sinZ = sinf(z * 0.5f);

	float qX = cosZ * sinX * cosY + sinZ * cosX * sinY;
	float qY = cosZ * cosX * sinY - sinZ * sinX * cosY;
	float qZ = sinZ * cosX * cosY - cosZ * sinX * sinY;
	float qW = cosZ * cosX * cosY + sinZ * sinX * sinY;

	return D3DXQUATERNION(qX, qY, qZ, qW);
}
/*******************************************************************************
*	�I�C���[�p���N�I�[�^�j�I���ɕϊ�(D3DXVECTOR3)
*******************************************************************************/
D3DXQUATERNION Math::EulerAngleToQuaternion(D3DXVECTOR3 eulerAngle)
{
	float cosX = cosf(eulerAngle.x * 0.5f);
	float sinX = sinf(eulerAngle.x * 0.5f);
	float cosY = cosf(eulerAngle.y * 0.5f);
	float sinY = sinf(eulerAngle.y * 0.5f);
	float cosZ = cosf(eulerAngle.z * 0.5f);
	float sinZ = sinf(eulerAngle.z * 0.5f);

	float qX = cosZ * sinX * cosY + sinZ * cosX * sinY;
	float qY = cosZ * cosX * sinY - sinZ * sinX * cosY;
	float qZ = sinZ * cosX * cosY - cosZ * sinX * sinY;
	float qW = cosZ * cosX * cosY + sinZ * sinX * sinY;

	return D3DXQUATERNION(qX, qY, qZ, qW);
}

/*******************************************************************************
*	�N�I�[�^�j�I�����I�C���[�p�ɕϊ�(float�����w��)
*******************************************************************************/
D3DXVECTOR3 Math::QuaternionToEulerAngle(float x, float y, float z, float w)
{
	float XX = x * x;
	float XY = x * y;
	float XZ = x * z;
	float XW = x * w;
	float YY = y * y;
	float YZ = y * z;
	float YW = y * w;
	float ZZ = z * z;
	float ZW = z * w;
	float WW = w * w;

	float sy = 2.0f * XZ + 2.0f * YW;
	bool unlocked = fabs(sy) < 0.99999f;

	float yaw = unlocked ? atan2f(-(2.0f * YZ - 2.0f * XW), 2.0f * WW + 2.0f * ZZ - 1.0f) :
		atan2f(2.0f * YZ + 2.0f + XW, 2.0f * WW + 2.0f * YY - 1.0f);
	float pitch = asinf(sy);
	float roll = unlocked ? atan2f(-(2.0f * XY - 2.0f * ZW), 2.0f * WW + 2.0f * XX - 1.0f) : 0.0f;

	if (!isfinite(yaw)) { yaw = 0.0f; }
	if (!isfinite(pitch)) { pitch = 0.0f; }
	if (!isfinite(roll)) { roll = 0.0f; }

	return D3DXVECTOR3(pitch, yaw, roll);
}
/*******************************************************************************
*	�N�I�[�^�j�I�����I�C���[�p�ɕϊ�(D3DXQUATERNION)
*******************************************************************************/
D3DXVECTOR3 Math::QuaternionToEulerAngle(D3DXQUATERNION quaternion)
{
	float XX = quaternion.x * quaternion.x;
	float XY = quaternion.x * quaternion.y;
	float XZ = quaternion.x * quaternion.z;
	float XW = quaternion.x * quaternion.w;
	float YY = quaternion.y * quaternion.y;
	float YZ = quaternion.y * quaternion.z;
	float YW = quaternion.y * quaternion.w;
	float ZZ = quaternion.z * quaternion.z;
	float ZW = quaternion.z * quaternion.w;
	float WW = quaternion.w * quaternion.w;

	float sy = 2.0f * XZ + 2.0f * YW;
	bool unlocked = fabs(sy) < 0.99999f;

	float yaw = unlocked ? atan2f(-(2.0f * YZ - 2.0f * XW), 2.0f * WW + 2.0f * ZZ - 1.0f) :
		atan2f(2.0f * YZ + 2.0f + XW, 2.0f * WW + 2.0f * YY - 1.0f);
	float pitch = asinf(sy);
	float roll = unlocked ? atan2f(-(2.0f * XY - 2.0f * ZW), 2.0f * WW + 2.0f * XX - 1.0f) : 0.0f;
	
	if (!isfinite(yaw)) { yaw = 0.0f; }
	if (!isfinite(pitch)) { pitch = 0.0f; }
	if (!isfinite(roll)) { roll = 0.0f; }

	return D3DXVECTOR3(yaw, pitch, roll);
}

//---------------------------------------
// �P�ʃN�I�[�^�j�I���쐬
//---------------------------------------
D3DXQUATERNION Math::GetQuaternionIdentity()
{
	D3DXQUATERNION q;
	D3DXQuaternionIdentity(&q);
	return q;
}

//---------------------------------------
// �P�ʃ}�g���N�X�쐬
//---------------------------------------
D3DXMATRIX Math::GetMatrixIdentity()
{
	D3DXMATRIX m;
	D3DXMatrixIdentity(&m);
	return m;
}

/*******************************************************************************
*	��]�N�I�[�^�j�I������(Radian�F���W�A��)
*******************************************************************************/
D3DXQUATERNION Math::AxisRotation_Radian(D3DXVECTOR3 axis, float value)
{
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &axis, value);
	return quat;
}
/*******************************************************************************
*	��]�N�I�[�^�j�I������(Degree�F�x���@)
*******************************************************************************/
D3DXQUATERNION Math::AxisRotation_Degree(D3DXVECTOR3 axis, float value)
{
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &axis, DegreeToRadian(value));
	return quat;
}

/*******************************************************************************
*	�x�N�g���̂Ȃ��p���v�Z(������ŋ߂�����)(D3DXVECTOR3)
*******************************************************************************/
float Math::VectorAngle(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	float dot = D3DXVec3Dot(&v1, &v2);
	if (dot > 1.0f) { dot = 1.0f; }
	else if (dot < -1.0f) { dot = -1.0f; }
	return acosf(dot);
}

/*******************************************************************************
*	�x�N�g���̂Ȃ��p���v�Z(������ŋ߂�����)(D3DXVECTOR2)
*******************************************************************************/
float Math::VectorAngle(D3DXVECTOR2 v1, D3DXVECTOR2 v2)
{
	float dot = D3DXVec2Dot(&v1, &v2);
	return acosf(dot);
}

/*******************************************************************************
*	2D�x�N�g���̂Ȃ��p���v�Z(�����)
*******************************************************************************/
float Math::Vector2DAngle_TurnLeft(D3DXVECTOR2 v1, D3DXVECTOR2 v2)
{
	float angle1 = atan2f(v1.y, v1.x);
	if (angle1 < 0.0f) { angle1 += m_PI<float> * 2.0f; }

	float angle2 = atan2f(v2.y, v2.x);
	if (angle2 < 0.0f) { angle2 += m_PI<float> * 2.0f; }

	float angleDistance = angle1 - angle2;
	if (angleDistance < 0.0f) { angleDistance += m_PI<float> * 2.0f; }

	return angleDistance;
}

/*******************************************************************************
*	2D�x�N�g���̂Ȃ��p���v�Z(�E���)
*******************************************************************************/
float Math::Vector2DAngle_TurnRight(D3DXVECTOR2 v1, D3DXVECTOR2 v2)
{
	float angleDistance = Vector2DAngle_TurnLeft(v1, v2);
	angleDistance = m_PI<float> * 2.0f - angleDistance;

	return angleDistance;
}
