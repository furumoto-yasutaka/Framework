/*******************************************************************************
*
*	タイトル：	シーンクラス
*	ファイル：	.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// コンポーネント
#include "camera.h"

// スクリプト
#include "debugCameraControl.h"

class TemplateScene : public Scene
{
public:
	~TemplateScene() {}

	void Init() override
	{
		GameObject* obj;

		// カメラ
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();
	}
};
