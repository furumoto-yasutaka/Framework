/*******************************************************************************
*
*	タイトル：	テスト用コンポーネント
*	ファイル：	testScript.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "testScript.h"
#include "gameObject.h"

void TestScript::Init()
{

}

void TestScript::Update()
{
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.5f);
}
