/*******************************************************************************
*
*	�^�C�g���F	��l�̎��_�J�����R���|�[�l���g
*	�t�@�C���F	firstPersonCamera.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "firstPersonCamera.h"
#include "renderer.h"
#include "gameObject.h"
#include "manager.h"
#include "input.h"

void FirstPersonCamera::Init(bool isActive, InputModeId inputMode,
	GameObject* followObject, D3DXVECTOR3 followOffset)
{
	PersonCamera::Init(isActive, inputMode, followObject, followOffset);
}

void FirstPersonCamera::Update()
{
	//-----------------
	// ���_����
	//-----------------
	// ���͂��擾
	m_RotateValue += GetRotateInput();
	// ���̊p�x�Ő���
	if (m_RotateValue.x > m_RotateMaxX) { m_RotateValue.x = m_RotateMaxX; }
	else if (m_RotateValue.y < -m_RotateMaxX) { m_RotateValue.y = -m_RotateMaxX; }
	// ��]�𔽉f
	D3DXQuaternionIdentity(&m_AttachObject->m_LocalRotation);
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_RotateValue.y);
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(m_AttachObject->GetRight(), m_RotateValue.x);
}

void FirstPersonCamera::LateUpdate()
{
	//-----------------
	// �J�����Ǐ]
	//-----------------
	if (m_FollowObject)
	{
		D3DXVECTOR3 forward, up, right;
		forward = m_FollowObject->GetForward() * m_FollowOffset.z;
		up = m_FollowObject->GetUp() * m_FollowOffset.y;
		right = m_FollowObject->GetRight() * m_FollowOffset.x;

		m_AttachObject->m_LocalPosition = m_FollowObject->m_LocalPosition + forward + up + right;
	}

	Camera::LateUpdate();
}

void FirstPersonCamera::DrawView()
{
	SetStructuredBuffer();

	// �r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 pos = m_AttachObject->m_LocalPosition;
	D3DXVECTOR3 target = m_AttachObject->m_LocalPosition + m_AttachObject->GetForward();
	D3DXMatrixLookAtLH(&m_ViewMatrix, &pos, &target, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);


	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
		Application::m_WINDOW_RESOLUTION.x / Application::m_WINDOW_RESOLUTION.y,
		0.1f, 1000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

#ifdef _DEBUG
/*******************************************************************************
*	�C���X�y�N�^�[�\������
*******************************************************************************/
void FirstPersonCamera::DrawInspector()
{
	if (ImGui::CollapsingHeader("FirstPersonCamera"))
	{
		PersonCamera::DrawInspector();
	}
}
#endif
