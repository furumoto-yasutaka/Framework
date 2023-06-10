/*******************************************************************************
*
*	�^�C�g���F	�f���p�I�u�W�F�N�g�ړ��X�N���v�g
*	�t�@�C���F	testInvoke.cpp
*	�쐬�ҁF	�Ö{ �ח�
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
