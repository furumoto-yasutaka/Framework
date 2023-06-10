/*******************************************************************************
*
*	タイトル：	三人称視点カメラコンポーネント
*	ファイル：	thirdPersonCamera.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "personCamera.h"

class ThirdPersonCamera : public PersonCamera
{
private:
	float		m_FollowSpeed = 0.4f;		// 補間速度(0.0f < v <= 1.0f)

	D3DXMATRIX m_CameraMatrix;				// カメラの姿勢

public:
	ThirdPersonCamera(GameObject* attachObject)
		: PersonCamera(attachObject)
	{}
	~ThirdPersonCamera() {}

	void Init(bool isActive, InputModeId inputMode, GameObject* followObject = NULL,
		D3DXVECTOR3 followOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void Update() override;
	void LateUpdate() override;
	void DrawView() override;

private:
	void SetStructuredBuffer() override;

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	float GetFollowSpeed() { return m_FollowSpeed; }
	void SetFollowSpeed(float speed) { m_FollowSpeed = speed; }
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
