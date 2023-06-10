/*******************************************************************************
*
*	�^�C�g���F	�C���X�y�N�^�[�E�B���h�E�N���X
*	�t�@�C���F	debugWindow_ObjectInspector.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#include "debugWindow_ObjectInspector.h"

void DebugWindow_ObjectInspector::Draw()
{
	ImGui::SetNextWindowPos(m_WindowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(m_WindowSize, ImGuiCond_Once);

	optional<int> index = DebugWindow_Hierarchy::GetSelectedObjectIndex();
	string tempStr = m_WindowName + "	";

	if (index)
	{// �q�G�����L�[����I�u�W�F�N�g���I������Ă���
		GameObject* selectedObj = DebugWindow_Hierarchy::GetSelectedObject();
		tempStr += to_string(index.value()) + " : " + selectedObj->GetName();

		// ���W�Œ�ݒ�ŃE�B���h�E��\��
		ImGui::Begin(tempStr.c_str(), ((bool*)0));

		DrawObjectInfo(selectedObj);
	}
	else
	{// �q�G�����L�[����I�u�W�F�N�g���I������Ă��Ȃ�
		// ���W�Œ�ݒ�ŃE�B���h�E��\��
		ImGui::Begin(tempStr.c_str(), ((bool*)0));
	}

	ImGui::End();
}

/*******************************************************************************
*	�q�G�����L�[�E�B���h�E�őI����ԂɂȂ��Ă���I�u�W�F�N�g��\��
*******************************************************************************/
void DebugWindow_ObjectInspector::DrawObjectInfo(GameObject* obj)
{
	obj->DrawInspector();

	DebugManager::SpacingAndSeparator();

	// �e�R���|�[�l���g�̃f�o�b�O����\��
	for (Component* c : obj->GetComponentAll())
	{
		c->DrawInspector();
	}
}
#endif
