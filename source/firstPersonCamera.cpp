/*******************************************************************************
*
*	タイトル：	一人称視点カメラコンポーネント
*	ファイル：	firstPersonCamera.cpp
*	作成者：	古本 泰隆
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
	// 視点操作
	//-----------------
	// 入力を取得
	m_RotateValue += GetRotateInput();
	// 一定の角度で制限
	if (m_RotateValue.x > m_RotateMaxX) { m_RotateValue.x = m_RotateMaxX; }
	else if (m_RotateValue.y < -m_RotateMaxX) { m_RotateValue.y = -m_RotateMaxX; }
	// 回転を反映
	D3DXQuaternionIdentity(&m_AttachObject->m_LocalRotation);
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_RotateValue.y);
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(m_AttachObject->GetRight(), m_RotateValue.x);
}

void FirstPersonCamera::LateUpdate()
{
	//-----------------
	// カメラ追従
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

	// ビューマトリクス設定
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 pos = m_AttachObject->m_LocalPosition;
	D3DXVECTOR3 target = m_AttachObject->m_LocalPosition + m_AttachObject->GetForward();
	D3DXMatrixLookAtLH(&m_ViewMatrix, &pos, &target, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);


	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
		Application::m_WINDOW_RESOLUTION.x / Application::m_WINDOW_RESOLUTION.y,
		0.1f, 1000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void FirstPersonCamera::DrawInspector()
{
	if (ImGui::CollapsingHeader("FirstPersonCamera"))
	{
		PersonCamera::DrawInspector();
	}
}
#endif
