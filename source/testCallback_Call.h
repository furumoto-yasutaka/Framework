/*******************************************************************************
*
*	タイトル：	コールバック機能テスト(呼び出し側)
*	ファイル：	testCallback_Call.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TestCallback_Call : public NotDrawComponent
{
private:
	CallBackBase<>* m_CallBack = NULL;

public:
	TestCallback_Call(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TestCallback_Call() {}

	void Init() {}
	void LateInit();
	void Uninit() override;
	void Update() override {}
	void LateUpdate() override {}
};
