/*******************************************************************************
*
*	タイトル：	カメラコンポーネント
*	ファイル：	camera.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "camera.h"
#include "renderer.h"
#include "gameObject.h"
#include "manager.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

void Camera::Init(bool isActive)
{
	m_CameraList.push_back(this);

	if (isActive)
	{
		// アクティブカメラに設定する
		Manager::GetScene()->SetActiveCamera(this);
	}
}

void Camera::Uninit()
{
	m_CameraList.remove(this);

	if (Manager::GetScene()->GetActiveCamera() == this)
	{
		Manager::GetScene()->DeleteActiveCamera();
	}
}

void Camera::DrawView()
{
	SetStructuredBuffer();

	// ビューマトリクス設定
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 pos = m_AttachObject->GetWorldPosition();
	D3DXVECTOR3 target = m_AttachObject->m_LocalPosition + m_AttachObject->GetForward();
	D3DXMatrixLookAtLH(&m_ViewMatrix, &pos, &target, &up);
	Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
		Application::m_WINDOW_RESOLUTION.x / Application::m_WINDOW_RESOLUTION.y,
		0.1f, 1000.0f);
	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

bool Camera::CheckFrustumCulling(D3DXVECTOR3 position)
{
	D3DXMATRIX vp, invvp;

	vp = m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	D3DXVECTOR3 v, v1, v2, n;

	// カメラからオブジェクトのベクトル
	v = position - m_AttachObject->m_LocalPosition;

	// 左→下→右→上の順で判定する
	for (int i = 0; i < 4; i++)
	{
		v1 = wpos[i] - m_AttachObject->m_LocalPosition;
		D3DXVec3Normalize(&v1, &v1);
		v2 = wpos[(i + 1) % 4] - m_AttachObject->m_LocalPosition;
		D3DXVec3Normalize(&v2, &v2);

		// 外積計算
		D3DXVec3Cross(&n, &v1, &v2);

		if (D3DXVec3Dot(&n, &v) < -3.0f)
		{
			return false;
		}
	}

	return true;
}

void Camera::SetStructuredBuffer()
{
	Renderer::SetCameraPosition(m_AttachObject->GetWorldPosition());
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void Camera::DrawInspector()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		DrawInspectorElem();
	}
}

void Camera::DrawInspectorElem()
{
	bool isActiveCamera = (Manager::GetScene()->GetActiveCamera() == this);
	ImGui::Text("IsActiveCamera : %s", (isActiveCamera ? "true" : "false"));

	ImGui::SameLine();

	if (ImGui::Button("SetActiveCamera"))
	{
		Manager::GetScene()->SetActiveCamera(this);
	}

	DebugManager::SpacingAndSeparator();
}
#endif
