/*******************************************************************************
*
*	�^�C�g���F	�l�̎��_�J�������N���X
*	�t�@�C���F	personCamera.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "camera.h"

#include <vector>
#include <string>

class PersonCamera : public Camera
{
public:
	enum class InputModeId
	{
		Mouse = 0,
		Controller,
	};

protected:
	InputModeId	m_InputMode = InputModeId::Mouse;
	GameObject* m_FollowObject = NULL;		// �ǔ��ΏۃI�u�W�F�N�g
	D3DXVECTOR3	m_FollowOffset;				// �ǔ����W�I�t�Z�b�g�l
	bool		m_IsRotateXReverse = false;	// X����]�̓��͔��]
	bool		m_IsRotateYReverse = false;	// Y����]�̓��͔��]
	D3DXVECTOR2	m_RotateSenseMouse = { 0.15f, 0.1f };		// ��]���x(�}�E�X)
	D3DXVECTOR2	m_RotateSenseController = { 2.0f, 1.0f };	// ��]���x(�R���g���[���[)
	float		m_RotateMaxX = 60.0f;						// ��]���(X)
	
	D3DXVECTOR2 m_RotateValue;				// ���[�J�������ւ̉�]��
public:
	D3DXVECTOR3 m_Offset;					// �O��������W�𑀍삷�邽�߂̃I�t�Z�b�g

public:
	PersonCamera(GameObject* attachObject)
		: Camera(attachObject)
	{}
	virtual ~PersonCamera() {}

	void Init(bool isActive, InputModeId inputMode, GameObject* followObject = NULL,
		D3DXVECTOR3 followOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	D3DXVECTOR2 GetRotateInput();

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	InputModeId GetInputMode() { return m_InputMode; }
	void SetInputMode(InputModeId id) { m_InputMode = id; }

	GameObject* GetFollowObject() { return m_FollowObject; }
	void SetFollowObject(GameObject* followObject) { m_FollowObject = followObject; }

	D3DXVECTOR3 GetFollowOffset() { return m_FollowOffset; }
	void SetFollowOffset(D3DXVECTOR3 followOffset) { m_FollowOffset = followOffset; }

	bool GetIsRotateXReverse() { return m_IsRotateXReverse; }
	void SetIsRotateXReverse(bool isReverse) { m_IsRotateXReverse = isReverse; }

	bool GetIsRotateYReverse() { return m_IsRotateYReverse; }
	void SetIsRotateYReverse(bool isReverse) { m_IsRotateYReverse = isReverse; }

	D3DXVECTOR2 GetRotateSenseMouse() { return m_RotateSenseMouse; }
	void SetRotateSenseMouse(D3DXVECTOR2 sense) { m_RotateSenseMouse = sense; }
	void SetRotateSenseMouseX(float senseX) { m_RotateSenseMouse.x = senseX; }
	void SetRotateSenseMouseY(float senseY) { m_RotateSenseMouse.y = senseY; }

	D3DXVECTOR2 GetRotateSenseController() { return m_RotateSenseController; }
	void SetRotateSenseController(D3DXVECTOR2 sense) { m_RotateSenseController = sense; }
	void SetRotateSenseControllerX(float senseX) { m_RotateSenseController.x = senseX; }
	void SetRotateSenseControllerY(float senseY) { m_RotateSenseController.y = senseY; }

	float GetRotateMaxX() { return m_RotateMaxX; }
	void SetRotateMaxX(float maxX) { m_RotateMaxX = maxX; }

	D3DXVECTOR2 GetRotateValue() { return m_RotateValue; }
	void SetRotateValue(D3DXVECTOR2 value) { m_RotateValue = value; }

	D3DXVECTOR3 GetOffset() { return m_Offset; }
	void SetOffset(D3DXVECTOR3 offset) { m_Offset = offset; }
	/////////////////////////////

#ifdef _DEBUG
protected:
	const vector<string> m_INPUT_MODE_STR = {
		"Mouse",
		"Controller",
	};

public:
	void DrawInspector() override;
#endif
};
