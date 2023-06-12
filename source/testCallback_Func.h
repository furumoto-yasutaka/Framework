/*******************************************************************************
*
*	�^�C�g���F	�R�[���o�b�N�@�\�e�X�g(�Ăяo����)
*	�t�@�C���F	testCallback_Func.h
*	�쐬�ҁF	�Ö{ �ח�
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
