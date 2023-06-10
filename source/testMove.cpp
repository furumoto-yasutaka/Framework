/*******************************************************************************
*
*	タイトル：	デモ用オブジェクト移動スクリプト
*	ファイル：	testInvoke.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "testMove.h"
#include "gameObject.h"

void TestMove::Init(D3DXVECTOR3 moveRange)
{
	m_FirstPos = m_AttachObject->m_LocalPosition;
	m_MoveRange = moveRange;
}

void TestMove::Update()
{
	m_Time += Time::GetDeltaTimeSec();

	m_AttachObject->m_LocalPosition = m_FirstPos + m_MoveRange * sinf(m_Time);
}
