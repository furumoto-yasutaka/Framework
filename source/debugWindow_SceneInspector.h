/*******************************************************************************
*
*	�^�C�g���F	�V�[���p�C���X�y�N�^�[�E�B���h�E�N���X
*	�t�@�C���F	debugWindow_SceneInspector.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "debugWindow.h"

class DebugWindow_SceneInspector : public DebugWindow
{
public:
	DebugWindow_SceneInspector(bool isActive = true)
		: DebugWindow(isActive)
	{
		m_WindowName = "SceneInspector";
		m_WindowPos = ImVec2(1050.0f, 20.0f);
		m_WindowSize = ImVec2(300.0f, 300.0f);
	}
	~DebugWindow_SceneInspector() {}

	void Draw() override;

	// �E�B���h�E�����
	void BeginWindow();
};
#endif
