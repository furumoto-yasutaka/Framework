/*******************************************************************************
*
*	タイトル：	一人称視点カメラコンポーネント
*	ファイル：	firstPersonCamera.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "personCamera.h"

class FirstPersonCamera : public PersonCamera
{
public:
	FirstPersonCamera(GameObject* attachObject)
		: PersonCamera(attachObject)
	{}
	~FirstPersonCamera() {}

	void Init(bool isActive, InputModeId inputMode, GameObject* followObject = NULL,
		D3DXVECTOR3 followOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void Update() override;
	void LateUpdate() override;
	void DrawView() override;

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
