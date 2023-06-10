/*******************************************************************************
*
*	�^�C�g���F	���w�I�֐��E�\���̗p�ÓI�N���X
*	�t�@�C���F	math.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include <d3dx9.h>

class Math
{
public:
	template<class T>
	struct MinMax
	{
		T Min;
		T Max;

		MinMax()
		{
			Min = T();
			Max = T();
		}
		MinMax(T min, T max)
		{
			this->Min = min;
			this->Max = max;
		}
	};

	template<typename T>
	static inline constexpr T m_PI = static_cast<T>(3.14159265358979323846);
	template<typename T>
	static inline constexpr T m_HALF_DIV_PI = static_cast<T>(57.29577951308232);		// 180 / PI
	template<typename T>	
	static inline constexpr T m_PI_DIV_HALF = static_cast<T>(0.017453292519943295);		// PI / 180

private:
	Math() {}

public:
	//-----------------------
	// �x�N�g���̐��K��
	//-----------------------
	static D3DXVECTOR3 Normalize(D3DXVECTOR3 vector);
	static D3DXVECTOR3 Normalize(float x, float y, float z);
	static D3DXVECTOR2 Normalize(D3DXVECTOR2 vector);
	static D3DXVECTOR2 Normalize(float x, float y);

	//-------------
	// ��������
	//-------------
	static int Random(int max);
	static float Random(float max);
	static D3DXVECTOR2 Random(D3DXVECTOR2 max);
	static D3DXVECTOR3 Random(D3DXVECTOR3 max);

	//---------------------------------------
	// �x���@���烉�W�A���ɕϊ�
	//---------------------------------------
	static float DegreeToRadian(float degree);				// (1��)
	static D3DXVECTOR3 DegreeToRadian(D3DXVECTOR3 degree);	// (3��)

	//---------------------------------------
	// ���W�A������x���@�ɕϊ�
	//---------------------------------------
	static float RadianToDegree(float radian);				// (1��)
	static D3DXVECTOR3 RadianToDegree(D3DXVECTOR3 radian);	// (3��)

	//---------------------------------------
	// �I�C���[�p���N�I�[�^�j�I���ɕϊ�
	//---------------------------------------
	static D3DXQUATERNION EulerAngleToQuaternion(float x, float y, float z);
	static D3DXQUATERNION EulerAngleToQuaternion(D3DXVECTOR3 eulerAngle);

	//---------------------------------------
	// �N�I�[�^�j�I�����I�C���[�p�ɕϊ�
	//---------------------------------------
	static D3DXVECTOR3 QuaternionToEulerAngle(float x, float y, float z, float w);
	static D3DXVECTOR3 QuaternionToEulerAngle(D3DXQUATERNION quaternion);

	//---------------------------------------
	// �P�ʍ쐬
	//---------------------------------------
	static D3DXQUATERNION GetQuaternionIdentity();
	static D3DXMATRIX GetMatrixIdentity();

	//---------------------------------------
	// ��]�N�I�[�^�j�I������
	//---------------------------------------
	static D3DXQUATERNION AxisRotation_Radian(D3DXVECTOR3 axis, float value);
	static D3DXQUATERNION AxisRotation_Degree(D3DXVECTOR3 axis, float value);

	//---------------------------------------
	// �x�N�g���̂Ȃ��p���v�Z(������ŋ߂�����)
	//---------------------------------------
	static float VectorAngle(D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	static float VectorAngle(D3DXVECTOR2 v1, D3DXVECTOR2 v2);

	//---------------------------------------
	// 2D�x�N�g���̂Ȃ��p���v�Z(�����)
	//---------------------------------------
	static float Vector2DAngle_TurnLeft(D3DXVECTOR2 v1, D3DXVECTOR2 v2);

	//---------------------------------------
	// 2D�x�N�g���̂Ȃ��p���v�Z(�E���)
	//---------------------------------------
	static float Vector2DAngle_TurnRight(D3DXVECTOR2 v1, D3DXVECTOR2 v2);
};
