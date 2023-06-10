/*******************************************************************************
*
*	タイトル：	テスト用コンポーネント
*	ファイル：	testScript.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TestScript : public NotDrawComponent
{
private:
	bool a = false;
	float time = 0.0f;

public:
	TestScript(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TestScript() {}

	void Init();
	void Uninit() override {}
	void Update() override;
};
