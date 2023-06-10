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
	m_CallBack = new CallBack(f, &TestCallback_Func::Func);
	m_CallBack->Execute();
}

void TestCallback_Call::Uninit()
{
	delete m_CallBack;
}
