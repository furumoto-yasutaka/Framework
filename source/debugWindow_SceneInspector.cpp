/*******************************************************************************
*
*	タイトル：	シーン用インスペクターウィンドウクラス
*	ファイル：	debugWindow_SceneInspector.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#ifdef _DEBUG
#include "debugWindow_SceneInspector.h"
#include "manager.h"

void DebugWindow_SceneInspector::Draw()
{
	ImGui::SetNextWindowPos(m_WindowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(m_WindowSize, ImGuiCond_Once);

	// 座標固定設定でウィンドウを表示
	ImGui::Begin(m_WindowName.c_str(), ((bool*)0));

	Manager::GetScene()->DrawInspector(ImGui::GetWindowPos(), ImGui::GetWindowSize());

	ImGui::End();
}
#endif
