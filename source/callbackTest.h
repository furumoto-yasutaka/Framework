/*******************************************************************************
*
*	タイトル：	コールバックテストシーンクラス
*	ファイル：	callbackTest.h
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
#include "testCallback_Call.h"
#include "testCallback_Func.h"

class CallBackTest : public Scene
{
public:
	~CallBackTest() {}

	void Init() override
	{
		GameObject* obj;


		obj = AddGameObject("CallBack");
		obj->AddComponent<TestCallback_Call>()->Init();
		obj->AddComponent<TestCallback_Func>()->Init();

		// カメラ
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();
	}
};
