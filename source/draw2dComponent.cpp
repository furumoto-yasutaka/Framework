/*******************************************************************************
*
*	タイトル：	2D描画コンポーネント基底クラス
*	ファイル：	draw2dComponent.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "draw2dComponent.h"
#include "manager.h"

Draw2DComponent::Draw2DComponent(GameObject* attachObject, int updatePriority, int drawPriority)
	: Component(attachObject, updatePriority),
	m_DrawPriority(drawPriority)
{
	m_DrawGroup = DrawGroup::Draw2D;

	m_DrawPriorityOrderComponents.insert({ m_DrawPriority, this });
}

Draw2DComponent::~Draw2DComponent()
{
	auto range = m_DrawPriorityOrderComponents.equal_range(m_DrawPriority);

	for (auto itr = range.first; itr != range.second; itr++)
	{
		if (itr->second == this)
		{
			m_DrawPriorityOrderComponents.erase(itr);
			break;
		}
	}
}
