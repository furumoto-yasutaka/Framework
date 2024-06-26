/*******************************************************************************
*
*	タイトル：	テクスチャアニメーションコンポーネント
*	ファイル：	textureAnimation.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"
#include "plate2dRenderer.h"
#include "plate3dRenderer.h"

class TextureAnimation : public NotDrawComponent
{
public:
	// アニメーション設定
	enum class AnimationMode
	{
		Loop = 0,		// ループ
		Once_ObjectDestroy,	// アニメーションが1週後削除
		Once_ObjectDisable,	// アニメーションが1週後非アクティブに
	};

private:
	AnimationMode	m_Mode = AnimationMode::Loop;	// アニメーション設定
	D3DXVECTOR2*	m_TexCoordBegin = NULL;			// テクスチャ座標(始点)
	D3DXVECTOR2*	m_TexCoordDistance = NULL;		// テクスチャ座標(終点)
	int				m_WidthDiv = NULL;				// 横分割数
	int				m_HeightDiv = NULL;				// 縦分割数
	int				m_AnimationInterval = NULL;		// アニメーションを進める周期
	int				m_FrameCount = NULL;			// フレームカウント

public:
	TextureAnimation(GameObject* attachObject)
		: NotDrawComponent(attachObject, -100)
	{}
	~TextureAnimation() {}

	void Init(Plate2DRenderer* connectComponent, AnimationMode mode, int animationInterval = 1);
	void Init(Plate3DRenderer* connectComponent, AnimationMode mode, int animationInterval = 1);
	void Uninit() override {}
	void Update() override;
};
