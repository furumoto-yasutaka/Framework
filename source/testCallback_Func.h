/*******************************************************************************
*
*	タイトル：	コールバック機能テスト(呼び出し側)
*	ファイル：	testCallback_Func.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TestCallback_Func : public NotDrawComponent
{
private:
	CallbackBase<>* m_Callback = NULL;

public:
	TestCallback_Func(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TestCallback_Func() {}

	void Init() {}
	void Uninit() override {}
	void Update() override {}
	void LateUpdate() override {}

	void Func();
};
