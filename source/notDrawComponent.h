/*******************************************************************************
*
*	タイトル：	スクリプト系コンポーネント基底クラス
*	ファイル：	notDrawComponent.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "component.h"

class NotDrawComponent : public Component
{
public:
	NotDrawComponent(GameObject* attachObject, int updatePriority = 0);
	virtual ~NotDrawComponent() {}
};
