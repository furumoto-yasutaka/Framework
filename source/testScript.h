/*******************************************************************************
*
*	�^�C�g���F	�e�X�g�p�R���|�[�l���g
*	�t�@�C���F	testScript.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TestScript : public NotDrawComponent
{
private:
	bool a = false;
	float time = 0.0f;

public:
	TestScript(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TestScript() {}

	void Init();
	void Uninit() override {}
	void Update() override;
};
