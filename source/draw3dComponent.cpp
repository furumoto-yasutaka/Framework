/*******************************************************************************
*
*	タイトル：	3D描画コンポーネント基底クラス
*	ファイル：	draw3dComponent.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "draw3dComponent.h"
#include "manager.h"

Draw3DComponent::Draw3DComponent(GameObject* attachObject, int updatePriority)
	: Component(attachObject, updatePriority)
{
	m_DrawGroup = DrawGroup::Draw3D;

	m_GroupComponents.push_back(this);
}

Draw3DComponent::~Draw3DComponent()
{
	m_GroupComponents.remove(this);
}
