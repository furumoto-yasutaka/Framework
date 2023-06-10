/*******************************************************************************
*
*	タイトル：	スクリプト
*	ファイル：	.h
*	作成者：	古本 泰隆
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
