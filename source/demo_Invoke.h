/*******************************************************************************
*
*	タイトル：	Invoke紹介シーンクラス
*	ファイル：	demo_Invoke.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// コンポーネント
#include "camera.h"
#include "plate2dRenderer.h"
#include "plate3dRenderer.h"
#include "aabbCollider3d.h"
#include "audioSpeaker.h"
#include "skyDome.h"

// スクリプト
#include "testInvoke.h"
#include "debugCameraControl.h"

class Demo_Invoke : public Scene
{
public:
	~Demo_Invoke() {}

	void Init() override
	{
		GameObject* obj;

		// スカイドーム
		obj = AddGameObject("SkyDome");
		obj->AddComponent<SkyDome>()->Init();

		obj = AddGameObject("Telop");
		obj->m_LocalPosition = { 0.0f, 570.0f, 0.0f };
		obj->AddComponent<Plate2DRenderer>()->Init("Telop_Invoke", { 700.0f, 300.0f });


		obj = AddGameObject("Tex");
		obj->AddComponent<Plate3DRenderer>()->Init("DevelopLogo", { 1.0f, 1.0f });
		obj->AddComponent<TestInvoke>()->Init();


		// カメラ
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();
	}
};
