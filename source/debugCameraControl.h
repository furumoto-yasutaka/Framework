/*******************************************************************************
*
*	�^�C�g���F	�f�o�b�O�p�J��������X�N���v�g
*	�t�@�C���F	debugCameraControl.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class DebugCameraControl : public NotDrawComponent
{
private:
	float		m_RotateSenseX = 0.15f;		// ��]���x(X)
	float		m_RotateSenseY = 0.1f;		// ��]���x(Y)
	float		m_RotateMaxX = 60.0f;		// ��]���(X)
	float		m_MoveSpeedKey = 0.15f;		// �ړ����x(�L�[)
	float		m_MoveSpeedDrag = 0.05f;	// �ړ����x(�}�E�X�h���b�O)
	float		m_MoveSpeedWheel = 0.05f;	// �ړ����x(�}�E�X�z�C�[��)

	float		m_RotateXValue = 0.0f;	// ���[�J��X�����ւ̉�]��
	float		m_RotateYValue = 0.0f;	// ���[�J��Y�����ւ̉�]��

public:
	DebugCameraControl(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~DebugCameraControl() {}

	void Init() {}
	void Uninit() override {}
	void Update() override;
};