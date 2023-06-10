/*******************************************************************************
*
*	タイトル：	スクリプト系コンポーネント基底クラス
*	ファイル：	notDrawComponent.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "notDrawComponent.h"
#include "manager.h"

NotDrawComponent::NotDrawComponent(GameObject* attachObject, int updatePriority)
	: Component(attachObject, updatePriority)
{
	m_DrawGroup = DrawGroup::NotDraw;
}
