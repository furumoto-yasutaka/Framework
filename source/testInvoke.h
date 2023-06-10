/*******************************************************************************
*
*	タイトル：	Invoke検証スクリプト
*	ファイル：	testInvoke.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TestInvoke : public NotDrawComponent
{
private:
	float m_Interval = 5.0f;
	float m_Speed = 0.01f;

public:
	TestInvoke(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TestInvoke() {}

	void Init();
	void Uninit() override {}
	void Update() override;

	void Reset();

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	float GetInterval() { return m_Interval; }
	void SetInterval(float interval) { m_Interval = interval; }

	float GetSpeed() { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = speed; }
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
