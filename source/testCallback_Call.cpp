/*******************************************************************************
*
*	�^�C�g���F	�R�[���o�b�N�@�\�e�X�g(�Ăяo����)
*	�t�@�C���F	testCallback_Call.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "testCallback_Call.h"
#include "testCallback_Func.h"
#include "gameObject.h"

void TestCallback_Call::LateInit()
{
	TestCallback_Func* f = m_AttachObject->GetComponent<TestCallback_Func>();
	m_Callback = new Callback(f, &TestCallback_Func::Func);
	m_Callback->Execute();
}

void TestCallback_Call::Uninit()
{
	delete m_Callback;
}
