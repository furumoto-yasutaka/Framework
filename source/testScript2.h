/*******************************************************************************
*
*	�^�C�g���F	�e�X�g�p�R���|�[�l���g
*	�t�@�C���F	testScript2.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TestScript2 : public NotDrawComponent
{
public:
	TestScript2(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TestScript2() {}

	void Init();
	void Uninit() override {}
	void Update() override;
};
