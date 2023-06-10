/*******************************************************************************
*
*	�^�C�g���F	�O�l�̎��_�J�����R���|�[�l���g
*	�t�@�C���F	thirdPersonCamera.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "thirdPersonCamera.h"
#include "renderer.h"
#include "gameObject.h"
#include "input.h"
#include "manager.h"

void ThirdPersonCamera::Init(bool isActive, InputModeId inputMode,
	GameObject* followObject, D3DXVECTOR3 followOffset)
{
	PersonCamera::Init(isActive, inputMode, followObject, followOffset);
}

void ThirdPersonCamera::Update()
{
	//-----------------
	// ���_����
	//-----------------
	// ���͂��擾
	m_RotateValue += GetRotateInput();
	// ���̊p�x�Ő���
	if (m_RotateValue.x > m_RotateMaxX) { m_RotateValue.x = m_RotateMaxX; }
	else if (m_RotateValue.x < -m_RotateMaxX) { m_RotateValue.x = -m_RotateMaxX; }
	// ��]�𔽉f
	D3DXQuaternionIdentity(&m_AttachObject->m_LocalRotation);
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_RotateValue.y);
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(m_AttachObject->GetRight(), m_RotateValue.x);
}

void ThirdPersonCamera::LateUpdate()
{
	//-----------------
	// �J�����Ǐ]
	//-----------------
	if (m_FollowObject)
	{
		D3DXVECTOR3 distance = (m_FollowObject->GetWorldPosition() + m_FollowOffset) - m_AttachObject->m_LocalPosition;
		m_AttachObject->m_LocalPosition += distance * m_FollowSpeed;
	}
	else
	{
		m_AttachObject->m_LocalPosition = m_FollowOffset;
	}

	Camera::LateUpdate();
}

void ThirdPersonCamera::DrawView()
{
	// �r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 follow = m_AttachObject->m_LocalPosition - m_FollowOffset;
	D3DXVECTOR3 offset = m_FollowOffset + m_Offset;
	D3DXMATRIX rot, trans1, trans2;
	D3DXMatrixTranslation(&trans1, follow.x, follow.y, follow.z);
	D3DXMatrixRotationQuaternion(&rot, &m_AttachObject->m_LocalRotation);
	D3DXMatrixTranslation(&trans2, offset.x, offset.y, offset.z);
	// �ǔ����̂ݍ��W���ړ� �� ���_�𒆐S�ɉ�] �� �ݒ肵���I�t�Z�b�g�����W���ړ�
	m_CameraMatrix = trans2 * rot * trans1;
	SetStructuredBuffer();

	// �r���[�}�g���N�X�̓J�����̈ړ��ł͂Ȃ�
	// �J�����ȊO�̑S�Ẳe���ɂȂ�̂ŋt�s���ݒ�
	D3DXMatrixInverse(&m_ViewMatrix, NULL, &m_CameraMatrix);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
		(float)WINDOW_RESOLUTION_WIDTH / WINDOW_RESOLUTION_HEIGHT,
		0.1f, 1000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void ThirdPersonCamera::SetStructuredBuffer()
{
	D3DXVECTOR3 p = { m_CameraMatrix._41, m_CameraMatrix._42, m_CameraMatrix._43 };
	Renderer::SetCameraPosition(p);
}

#ifdef _DEBUG
/*******************************************************************************
*	�C���X�y�N�^�[�\������
*******************************************************************************/
void ThirdPersonCamera::DrawInspector()
{
	if (ImGui::CollapsingHeader("ThirdPersonCamera"))
	{
		PersonCamera::DrawInspector();

		ImGui::SliderFloat("FollowSpeed", &m_FollowSpeed, 0.01f, 1.0f);
	}
}
#endif
