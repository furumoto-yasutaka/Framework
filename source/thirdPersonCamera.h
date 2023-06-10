/*******************************************************************************
*
*	�^�C�g���F	�O�l�̎��_�J�����R���|�[�l���g
*	�t�@�C���F	thirdPersonCamera.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "personCamera.h"

class ThirdPersonCamera : public PersonCamera
{
private:
	float		m_FollowSpeed = 0.4f;		// ��ԑ��x(0.0f < v <= 1.0f)

	D3DXMATRIX m_CameraMatrix;				// �J�����̎p��

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
	//�@�����@�A�N�Z�T�@�����@//
	float GetFollowSpeed() { return m_FollowSpeed; }
	void SetFollowSpeed(float speed) { m_FollowSpeed = speed; }
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
