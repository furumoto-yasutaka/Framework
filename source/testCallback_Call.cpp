/*******************************************************************************
*
*	タイトル：	コールバック機能テスト(呼び出し側)
*	ファイル：	testCallback_Call.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "testCallback_Call.h"
#include "testCallback_Func.h"
#include "gameObject.h"

void TestCallback_Call::LateInit()
{
	TestCallback_Func* f = m_AttachObject->GetComponent<TestCallback_Func>();
	m_CallBack = new CallBack(f, &TestCallback_Func::Func);
	m_CallBack->Execute();
}

void TestCallback_Call::Uninit()
{
	delete m_CallBack;
}
