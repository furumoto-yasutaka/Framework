/*******************************************************************************
*
*	タイトル：	デバッグウィンドウ制御クラス
*	ファイル：	debugWindow.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "main.h"
#include "debugManager.h"

class DebugWindow
{
protected:
	string		m_WindowName;		// ウィンドウ名
	bool		m_IsActive;			// ウィンドウが起動しているか
	ImVec2		m_WindowSize;		// ウィンドウサイズ
	ImVec2		m_WindowPos;		// ウィンドウサイズ
	D3DXVECTOR4 m_WindowClearColor;	// ウィンドウの色

public:
	DebugWindow(bool isActive = true)
	{
		m_IsActive = isActive;
		m_WindowPos = ImVec2(360.0f, 50.0f);
		m_WindowSize = ImVec2(200.0f, 300.0f);
		m_WindowClearColor = D3DXVECTOR4(0.45f, 0.55f, 0.60f, 1.00f);
	}
	virtual ~DebugWindow() {}

	virtual void Draw() = 0;

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	string GetName() { return m_WindowName; }

	bool GetIsActive() { return m_IsActive; }
	void SetIsActive(bool isActive) { m_IsActive = isActive; }
	/////////////////////////////
};
#endif
