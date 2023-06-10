/*******************************************************************************
*
*	�^�C�g���F	���S�\����ʊǗ��X�N���v�g
*	�t�@�C���F	developLogoManager.h
*	�쐬�ҁF	�Ö{ �ח�
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
