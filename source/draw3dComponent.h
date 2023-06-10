/*******************************************************************************
*
*	タイトル：	3D描画コンポーネント基底クラス
*	ファイル：	draw3dComponent.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "component.h"

class Draw3DComponent : public Component
{
protected:
	// 描画処理用
	static inline list<Draw3DComponent*> m_GroupComponents;

public:
	Draw3DComponent(GameObject* attachObject, int updatePriority = 0);
	virtual ~Draw3DComponent();

	virtual void Draw3d() = 0;

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	static inline list<Draw3DComponent*> GetGroupComponents()
	{
		return m_GroupComponents;
	}
	/////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawDebug3d() {}
#endif
};
