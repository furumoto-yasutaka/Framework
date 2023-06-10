/*******************************************************************************
*
*	タイトル：	タイトルシーンクラス
*	ファイル：	title.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// コンポーネント
#include "camera.h"
#include "plate2dRenderer.h"

// スクリプト
#include "debugCameraControl.h"

class Title : public Scene
{
public:
	~Title() {}

	void Init() override
	{
		GameObject* obj;

		obj = AddGameObject("Manager");

		// 背景
		obj = AddGameObject("BackGround");
		obj->AddComponent<Plate2DRenderer>()->Init("White",
			{ WINDOW_RESOLUTION_WIDTH, WINDOW_RESOLUTION_HEIGHT });

		// タイトルロゴ
		obj = AddGameObject("GameLogo");
		obj->AddComponent<Plate2DRenderer>()->Init("GameLogo",
			{ 1000.0f, 650.0f });

		// 催促テキスト
		obj = AddGameObject("PromptText");
		obj->AddComponent<Plate2DRenderer>()->Init("Title_PromptText",
			{ 600.0f, 80.0f });

		// カメラ
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
#ifdef _DEBUG
		obj->AddComponent<DebugCameraControl>()->Init();
#endif
	}
};

// 催促テキストの点滅
// ボタンの受付と自動ロゴ表示戻り(ひかめきからこぴぺ)
