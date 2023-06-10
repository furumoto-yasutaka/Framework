/*******************************************************************************
*
*	タイトル：	デバッグウィンドウ制御静的クラス
*	ファイル：	debugManager.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "main.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <list>
#include <vector>
#include <string>

class DebugWindow;

class DebugManager
{
	struct SceneButtonInfo
	{
		string Name;
		bool IsWireFrame;
		bool IsParentLine;
		bool IsCollisionFrame;
		bool IsSpaceDivisionFrame;
		void(* ChangeFunc)();
	};

private:
	static inline const ImVec2 m_WINDOW_POS = { 20.0f, 20.0f };
	static inline const ImVec2 m_WINDOW_SIZE = { 300.0f, 300.0f };
	static inline const float m_LIST_WIDTH = 250.0f;
	static inline const float m_LIST_BLANK_HEIGHT = 12.0f;
	static inline const float m_LIST_LINE_HEIGHT = 25.0f;

	static inline list<DebugWindow*> m_WindowList;
	static inline int m_CaptionIndex = 1;

	static inline float m_UseMemory[180];
	static inline float m_MaxMemory = 0.0f;

public:
	static inline bool m_IsWireFrame = false;				// ワイヤーフレーム
	static inline bool m_IsParentLine = true;				// オブジェクトとの親子関係
	static inline bool m_IsCollisionFrame = true;			// 当たり判定のフレーム
	static inline bool m_IsSpaceDivisionFrame = false;		// 空間分割のフレーム

private:
	DebugManager() {}
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();	// 各ウィンドウの表示制御用のウィンドウを作りたい

	static void DrawBegin();
	static void DrawEnd();

	// メモリ使用量表示
	static void DrawMemory();
	// デバッグ機能チェックボックス表示
	static void DrawDebugFlag();
	// デバッグウィンドウチェックボックス表示
	static void DrawWindowFlag();
	// デモシーン切り替えボタン表示
	static void DrawDemoSceneSelectButton();
	// ゲームシーン切り替えボタン表示
	static void DrawGameSceneSelectButton();

	// シーン切り替え(デモ)
	static void ChangeScene_Demo_Invoke();
	static void ChangeScene_Demo_Parent();
	static void ChangeScene_Demo_CollisionSplit();
	static void ChangeScene_Demo_GpuParticle();
	static void ChangeScene_Demo_StreamingAudio();

	// シーン切り替え(ゲーム)
	static void ChangeScene_Game_DevelopLogo();

	// 便利機能
	static void SpacingAndSeparator();
	static bool ComboBox(string caption,
		int& currentIndex, vector<string> items);

private:
	static inline const vector<SceneButtonInfo> m_DEMO_SCENE_INFO =
	{
		{ "Demo_Invoke", false, true, true, false, &ChangeScene_Demo_Invoke },
		{ "Demo_Parent & CollisionFrame", false, true, true, false, &ChangeScene_Demo_Parent },
		{ "Demo_CollisionSplit", false, false, true, true, &ChangeScene_Demo_CollisionSplit },
		{ "Demo_GpuParticle", false, false, false, false, &ChangeScene_Demo_GpuParticle },
		{ "Demo_StreamingAudio", false, false, false, false, &ChangeScene_Demo_StreamingAudio },
	};

	static inline const vector<SceneButtonInfo> m_GAME_SCENE_INFO =
	{
		{ "DevelopLogo", false, false, true, true, &ChangeScene_Game_DevelopLogo },
	};
};
#endif
