/*******************************************************************************
*
*	タイトル：	システム系コンポーネント基底クラス
*	ファイル：	cameraComponent.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "component.h"

class CameraComponent : public Component
{
protected:
	// 描画処理用
	static inline list<CameraComponent*> m_GroupComponents;

public:
	CameraComponent(GameObject* attachObject, int updatePriority = 0);
	virtual ~CameraComponent();

	virtual void DrawView() = 0;

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	static inline list<CameraComponent*> GetGroupComponents()
	{
		return m_GroupComponents;
	}
	/////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawDebug3d() {}
#endif
};
