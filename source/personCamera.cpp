/*******************************************************************************
*
*	タイトル：	人称視点カメラ基底クラス
*	ファイル：	personCamera.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "personCamera.h"
#include "input.h"
#include "gameObject.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

void PersonCamera::Init(bool isActive, InputModeId inputMode,
	GameObject* followObject, D3DXVECTOR3 followOffset)
{
	Camera::Init(isActive);
	m_InputMode = inputMode;
	m_FollowObject = followObject;
	m_FollowOffset = followOffset;
	m_RotateValue = { 0.0f, 0.0f };

	m_Offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

D3DXVECTOR2 PersonCamera::GetRotateInput()
{
	D3DXVECTOR2 v;

	switch (m_InputMode)
	{
		using enum InputModeId;
	case Mouse:
		v = { Input::GetMouseMoveY() * m_RotateSenseMouse.y,
			Input::GetMouseMoveX() * m_RotateSenseMouse.x };
		break;
	case Controller:
		v = { Input::GetGamePadStickRightY() * m_RotateSenseController.y,
			Input::GetGamePadStickRightX() * m_RotateSenseController.x };
		break;
	default:
		v = { 0.0f, 0.0f };
		break;
	}

	if (m_IsRotateXReverse) { v.x = -v.x; }
	if (m_IsRotateYReverse) { v.y = -v.y; }

	return v;
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void PersonCamera::DrawInspector()
{
	Camera::DrawInspectorElem();

	int tempInt;
	float tempVec2[2];
	float tempVec3[3];

	tempInt = (int)m_InputMode;
	if (DebugManager::ComboBox("InputModeId", tempInt, m_INPUT_MODE_STR))
	{
		m_InputMode = (PersonCamera::InputModeId)tempInt;
	}

	ImGui::Text("FollowObject : %s", (m_FollowObject ? m_FollowObject->GetName().c_str() : "None"));

	tempVec3[0] = m_FollowOffset.x;
	tempVec3[1] = m_FollowOffset.y;
	tempVec3[2] = m_FollowOffset.z;
	if (ImGui::InputFloat3("FollowOffset", tempVec3))
	{
		m_FollowOffset = { tempVec3[0], tempVec3[1], tempVec3[2] };
	}

	ImGui::Checkbox("IsRotateXReverse", &m_IsRotateXReverse);

	ImGui::Checkbox("IsRotateYReverse", &m_IsRotateYReverse);

	tempVec2[0] = m_RotateSenseMouse.x;
	tempVec2[1] = m_RotateSenseMouse.y;
	if (ImGui::InputFloat2("RotateSenceMouse", tempVec2))
	{
		m_RotateSenseMouse = { tempVec2[0], tempVec2[1] };
	}

	tempVec2[0] = m_RotateSenseController.x;
	tempVec2[1] = m_RotateSenseController.y;
	if (ImGui::InputFloat2("RotateSenceController", tempVec2))
	{
		m_RotateSenseController = { tempVec2[0], tempVec2[1] };
	}

	ImGui::SliderFloat("RotateMaxX", &m_RotateMaxX, 0.0f, 89.9f);

	tempVec2[0] = m_RotateValue.x;
	tempVec2[1] = m_RotateValue.y;
	if (ImGui::InputFloat2("RotateValue", tempVec2))
	{
		m_RotateValue = { tempVec2[0], tempVec2[1] };
	}

	tempVec3[0] = m_Offset.x;
	tempVec3[1] = m_Offset.y;
	tempVec3[2] = m_Offset.z;
	if (ImGui::InputFloat3("Offset", tempVec3))
	{
		m_Offset = { tempVec3[0], tempVec3[1], tempVec3[2] };
	}
}
#endif
