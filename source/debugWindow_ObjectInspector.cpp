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
	optional<int> index = DebugWindow_Hierarchy::GetSelectedObjectIndex();
	GameObject* selectedObj = BeginWindow(index);

	if (index)
	{
		DrawObjectInfo(selectedObj);
	}

	ImGui::End();
}

/*******************************************************************************
*	�E�B���h�E�����
*******************************************************************************/
GameObject* DebugWindow_ObjectInspector::BeginWindow(optional<int> index)
{
	ImGui::SetNextWindowPos(m_WindowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(m_WindowSize, ImGuiCond_Once);

	string tempStr = m_WindowName + "	";
	GameObject* selectedObj = NULL;

	if (index)
	{// �q�G�����L�[����I�u�W�F�N�g���I������Ă���
		selectedObj = DebugWindow_Hierarchy::GetSelectedObject();
		tempStr += to_string(index.value()) + " : " + selectedObj->GetName();
	}

	// ���W�Œ�ݒ�ŃE�B���h�E��\��
	ImGui::Begin(tempStr.c_str(), ((bool*)0));

	return selectedObj;
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
