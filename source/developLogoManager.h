/*******************************************************************************
*
*	タイトル：	ロゴ表示画面管理スクリプト
*	ファイル：	developLogoManager.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"

class DevelopLogoManager : public NotDrawComponent
{
private:
	static inline const float m_TimeLimit = 3.0f;
	CallBackInvokeBase* m_InvokeInstance = NULL;
	bool m_IsNextScene = false;

public:
	DevelopLogoManager(GameObject* attachObject)
		: NotDrawComponent(attachObject)
	{}
	~DevelopLogoManager() {}

	void Init();
	void Uninit() override {}
	void Update() override;
	void LateUpdate() override {}

	void ChangeTitleScene();
};
