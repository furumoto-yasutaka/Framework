/*******************************************************************************
*
*	タイトル：	テスト用コンポーネント
*	ファイル：	testScript2.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TestScript2 : public NotDrawComponent
{
public:
	TestScript2(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TestScript2() {}

	void Init();
	void Uninit() override {}
	void Update() override;
};
