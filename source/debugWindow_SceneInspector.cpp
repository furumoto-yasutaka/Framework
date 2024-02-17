/*******************************************************************************
*
*	�^�C�g���F	�V�[���p�C���X�y�N�^�[�E�B���h�E�N���X
*	�t�@�C���F	debugWindow_SceneInspector.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#include "debugWindow_SceneInspector.h"
#include "manager.h"

void DebugWindow_SceneInspector::Draw()
{
	BeginWindow();
	Manager::GetScene()->DrawInspector(ImGui::GetWindowPos(), ImGui::GetWindowSize());

	ImGui::End();
}

void DebugWindow_SceneInspector::BeginWindow()
{
	ImGui::SetNextWindowPos(m_WindowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(m_WindowSize, ImGuiCond_Once);

	// ���W�Œ�ݒ�ŃE�B���h�E��\��
	ImGui::Begin(m_WindowName.c_str(), ((bool*)0));

}
#endif
