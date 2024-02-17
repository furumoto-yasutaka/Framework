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
	optional<int> index = DebugWindow_Hierarchy::GetSelectedObjectIndex();
	GameObject* selectedObj = BeginWindow(index);

	if (index)
	{
		DrawObjectInfo(selectedObj);
	}

	ImGui::End();
}

/*******************************************************************************
*	ウィンドウを作る
*******************************************************************************/
GameObject* DebugWindow_ObjectInspector::BeginWindow(optional<int> index)
{
	ImGui::SetNextWindowPos(m_WindowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(m_WindowSize, ImGuiCond_Once);

	string tempStr = m_WindowName + "	";
	GameObject* selectedObj = NULL;

	if (index)
	{// ヒエラルキーからオブジェクトが選択されている
		selectedObj = DebugWindow_Hierarchy::GetSelectedObject();
		tempStr += to_string(index.value()) + " : " + selectedObj->GetName();
	}

	// 座標固定設定でウィンドウを表示
	ImGui::Begin(tempStr.c_str(), ((bool*)0));

	return selectedObj;
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
