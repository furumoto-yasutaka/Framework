/*******************************************************************************
*
*	タイトル：	インスペクターウィンドウクラス
*	ファイル：	debugWindow_ObjectInspector.cpp
*	作成者：	古本 泰隆
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
	{// ヒエラルキーからオブジェクトが選択されている
		GameObject* selectedObj = DebugWindow_Hierarchy::GetSelectedObject();
		tempStr += to_string(index.value()) + " : " + selectedObj->GetName();

		// 座標固定設定でウィンドウを表示
		ImGui::Begin(tempStr.c_str(), ((bool*)0));

		DrawObjectInfo(selectedObj);
	}
	else
	{// ヒエラルキーからオブジェクトが選択されていない
		// 座標固定設定でウィンドウを表示
		ImGui::Begin(tempStr.c_str(), ((bool*)0));
	}

	ImGui::End();
}

/*******************************************************************************
*	ヒエラルキーウィンドウで選択状態になっているオブジェクトを表示
*******************************************************************************/
void DebugWindow_ObjectInspector::DrawObjectInfo(GameObject* obj)
{
	obj->DrawInspector();

	DebugManager::SpacingAndSeparator();

	// 各コンポーネントのデバッグ情報を表示
	for (Component* c : obj->GetComponentAll())
	{
		c->DrawInspector();
	}
}
#endif
