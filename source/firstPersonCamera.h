/*******************************************************************************
*
*	�^�C�g���F	��l�̎��_�J�����R���|�[�l���g
*	�t�@�C���F	firstPersonCamera.h
*	�쐬�ҁF	�Ö{ �ח�
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
