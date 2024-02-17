/*******************************************************************************
*
*	タイトル：	ヒエラルキーウィンドウクラス
*	ファイル：	debugWindow_Hierarchy.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "debugWindow.h"

#include <list>
#include <vector>
#include <optional>
#include <string>

class GameObject;

class DebugWindow_Hierarchy : public DebugWindow
{
private:
	static inline vector<GameObject*> m_ObjList;
	static inline int m_DigitNum = 0;
public:
	static inline int m_SelectedObjectId = -1;

public:
	DebugWindow_Hierarchy(bool isActive = true)
		: DebugWindow(isActive)
	{
		m_WindowName = "Hierarchy";
		m_WindowPos = ImVec2(740.0f, 20.0f);
		m_WindowSize = ImVec2(300.0f, 300.0f);
	}
	~DebugWindow_Hierarchy() {}

	void Draw() override;

	// ウィンドウを作る
	void BeginWindow();

	// オブジェクト数から最大桁数を求める
	void CountDigit(int num);

	// オブジェクトをすべて表示する
	void DrawObjList(list<GameObject*> objList);

	// 子を持つオブジェクトを表示
	void DrawTree(GameObject* obj);

	// 子を持たないオブジェクトを表示
	void DrawName(GameObject* obj);

	// 表示対象ではない子オブジェクトを格納
	void PushChild(GameObject* obj);

	// 最大桁数から数値を生成
	string CreateNumStr(int num);

	// オブジェクトをリストから削除
	static void DeleteObj(GameObject* obj);

	// 選択状態のオブジェクトの情報を取得
	static GameObject* GetSelectedObject();

	// オブジェクトのヒエラルキー上の番号を取得
	static optional<int> GetObjectIndex(GameObject* obj);

	// 選択状態のオブジェクトのヒエラルキー上の番号を取得
	static optional<int> GetSelectedObjectIndex();
};
#endif
