/*******************************************************************************
*
*	タイトル：	シーン用インスペクターウィンドウクラス
*	ファイル：	debugWindow_SceneInspector.h
*	作成者：	古本 泰隆
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

	// ウィンドウを作る
	void BeginWindow();
};
#endif
