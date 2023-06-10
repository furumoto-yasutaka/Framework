/*******************************************************************************
*
*	タイトル：	オブジェクト用インスペクターウィンドウクラス
*	ファイル：	debugWindow_ObjectInspector.h
*	作成者：	古本 泰隆
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

	// ヒエラルキーウィンドウで選択状態になっているオブジェクトを表示
	void DrawObjectInfo(GameObject* obj);
};
#endif
