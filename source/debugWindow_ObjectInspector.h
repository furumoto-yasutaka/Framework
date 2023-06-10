/*******************************************************************************
*
*	�^�C�g���F	�I�u�W�F�N�g�p�C���X�y�N�^�[�E�B���h�E�N���X
*	�t�@�C���F	debugWindow_ObjectInspector.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "debugWindow.h"
#include "gameObject.h"
#include "math.h"
#include "debugWindow_Hierarchy.h"

#include <list>

class DebugWindow_ObjectInspector : public DebugWindow
{
public:
	DebugWindow_ObjectInspector(bool isActive = true)
		: DebugWindow(isActive)
	{
		m_WindowName = "ObjectInspector";
		m_WindowPos = ImVec2(330.0f, 20.0f);
		m_WindowSize = ImVec2(400.0f, 300.0f);
	}
	~DebugWindow_ObjectInspector() {}

	void Draw() override;

	// �q�G�����L�[�E�B���h�E�őI����ԂɂȂ��Ă���I�u�W�F�N�g��\��
	void DrawObjectInfo(GameObject* obj);
};
#endif
