/*******************************************************************************
*
*	タイトル：	数学的関数・構造体用静的クラス
*	ファイル：	math.h
*	作成者：	古本 泰隆
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
	// ベクトルの正規化
	//-----------------------
	static D3DXVECTOR3 Normalize(D3DXVECTOR3 vector);
	static D3DXVECTOR3 Normalize(float x, float y, float z);
	static D3DXVECTOR2 Normalize(D3DXVECTOR2 vector);
	static D3DXVECTOR2 Normalize(float x, float y);

	//-------------
	// 乱数生成
	//-------------
	static int Random(int max);
	static float Random(float max);
	static D3DXVECTOR2 Random(D3DXVECTOR2 max);
	static D3DXVECTOR3 Random(D3DXVECTOR3 max);

	//---------------------------------------
	// 度数法からラジアンに変換
	//---------------------------------------
	static float DegreeToRadian(float degree);				// (1軸)
	static D3DXVECTOR3 DegreeToRadian(D3DXVECTOR3 degree);	// (3軸)

	//---------------------------------------
	// ラジアンから度数法に変換
	//---------------------------------------
	static float RadianToDegree(float radian);				// (1軸)
	static D3DXVECTOR3 RadianToDegree(D3DXVECTOR3 radian);	// (3軸)

	//---------------------------------------
	// オイラー角をクオータニオンに変換
	//---------------------------------------
	static D3DXQUATERNION EulerAngleToQuaternion(float x, float y, float z);
	static D3DXQUATERNION EulerAngleToQuaternion(D3DXVECTOR3 eulerAngle);

	//---------------------------------------
	// クオータニオンをオイラー角に変換
	//---------------------------------------
	static D3DXVECTOR3 QuaternionToEulerAngle(float x, float y, float z, float w);
	static D3DXVECTOR3 QuaternionToEulerAngle(D3DXQUATERNION quaternion);

	//---------------------------------------
	// 単位作成
	//---------------------------------------
	static D3DXQUATERNION GetQuaternionIdentity();
	static D3DXMATRIX GetMatrixIdentity();

	//---------------------------------------
	// 回転クオータニオン生成
	//---------------------------------------
	static D3DXQUATERNION AxisRotation_Radian(D3DXVECTOR3 axis, float value);
	static D3DXQUATERNION AxisRotation_Degree(D3DXVECTOR3 axis, float value);

	//---------------------------------------
	// ベクトルのなす角を計算(両周りで近い方向)
	//---------------------------------------
	static float VectorAngle(D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	static float VectorAngle(D3DXVECTOR2 v1, D3DXVECTOR2 v2);

	//---------------------------------------
	// 2Dベクトルのなす角を計算(左回り)
	//---------------------------------------
	static float Vector2DAngle_TurnLeft(D3DXVECTOR2 v1, D3DXVECTOR2 v2);

	//---------------------------------------
	// 2Dベクトルのなす角を計算(右回り)
	//---------------------------------------
	static float Vector2DAngle_TurnRight(D3DXVECTOR2 v1, D3DXVECTOR2 v2);
};
