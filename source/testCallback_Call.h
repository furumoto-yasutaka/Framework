/*******************************************************************************
*
*	�^�C�g���F	�R�[���o�b�N�@�\�e�X�g(�Ăяo����)
*	�t�@�C���F	testCallback_Call.h
*	�쐬�ҁF	�Ö{ �ח�
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
