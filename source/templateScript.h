/*******************************************************************************
*
*	�^�C�g���F	�X�N���v�g
*	�t�@�C���F	.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class TemplateScript : public NotDrawComponent
{
private:


public:
	TemplateScript(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~TemplateScript() {}

	void Init() {}
	void Uninit() override {}
	void Update() override {}
	void LateUpdate() override {}
};
