/*******************************************************************************
*
*	タイトル：	2D描画コンポーネント基底クラス
*	ファイル：	draw2dComponent.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "component.h"

class Draw2DComponent : public Component
{
protected:
	// 描画処理用
	static inline multimap<int, Draw2DComponent*> m_DrawPriorityOrderComponents;

	int m_DrawPriority;		// 描画優先度

public:
	Draw2DComponent(GameObject* attachObject, int updatePriority = 0, int drawPriority = 0);
	virtual ~Draw2DComponent();

	virtual void Draw2d() = 0;

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	static inline multimap<int, Draw2DComponent*> GetDrawPriorityOrderComponents()
	{
		return m_DrawPriorityOrderComponents;
	}

	int GetDrawPriority() { return m_DrawPriority; }
	/////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawDebug2d() {}
#endif
};
