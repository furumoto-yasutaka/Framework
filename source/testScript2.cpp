/*******************************************************************************
*
*	タイトル：	テスト用コンポーネント
*	ファイル：	testScript2.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "testScript2.h"
#include "gameObject.h"

void TestScript2::Init()
{

}

void TestScript2::Update()
{
	m_AttachObject->m_LocalRotation *= Math::AxisRotation_Degree(D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.8f);
}
