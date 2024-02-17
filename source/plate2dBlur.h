/*******************************************************************************
*
*	タイトル：	テクスチャブラーコンポーネント
*	ファイル：	plate2dBlur.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "draw2dComponent.h"
#include "renderer.h"

#include <list>

class Plate2DRenderer;

class Plate2DBlur : public Draw2DComponent
{
private:
	Plate2DRenderer*	m_Plate2DRenderer = NULL;		// 連携中の2D描画コンポーネント
	ID3D11Buffer*		m_VertexBuffer = NULL;			// 頂点バッファ
	ID3D11VertexShader* m_VertexShader = NULL;			// 頂点シェーダー
	ID3D11PixelShader*	m_PixelShader = NULL;			// ピクセルシェーダー
	ID3D11InputLayout*	m_VertexLayout = NULL;			// 入力レイアウト

	int					m_BulrLayer = 0;				// ブラー枚数
	int					m_BulrInterval = 0;				// ブラー発生間隔(フレーム)
	int					m_InterpolationLayer = 0;		// ブラー補間枚数
	float				m_BulrAlphaDecayRate = 0.0f;	// 1枚当たりのアルファ減衰率
	int					m_FrameCount = 0;				// フレームカウンタ

	list<D3DXVECTOR2>	m_BulrPosition;					// ブラー発生位置
	list<float>			m_BulrRotationZ;				// ブラーの回転

public:
	Plate2DBlur(GameObject* AttachObject)
		: Draw2DComponent(AttachObject, -100)
	{}
	~Plate2DBlur() {}

	void Init(Plate2DRenderer* plate2D, unsigned int bulrLayer, unsigned int bulrInterval, unsigned int interpolationBlur = 0);
	void Uninit() override;
	void Update() override;
	void Draw2d() override;

private:
	void CreateBuffer();
	void CreateShader();

	// 最初に生成したブラーを削除する
	void ClearBackBulr();
	// ブラーを追加
	void CreateBulr();

	// 頂点情報を取得
	void GetVertex(VERTEX_3D* vertex, D3DXVECTOR2 pos, float alpha, float rot);

	// 頂点情報を生成
	void CreateVertex(VERTEX_3D* vertex, D3DXVECTOR2 pos, D3DXVECTOR2 offset, float alpha, float rot);

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	int GetBlurLayer() { return m_BulrLayer; }
	void SetBlurLayer(int Layer) { m_BulrLayer = Layer; }

	int GetBulrInterval() { return m_BulrInterval; }
	void SetBlurInterval(int Interval) { m_BulrInterval = Interval; }

	int GetInterpolationLayer() { return m_InterpolationLayer; }
	void SetInterpolationLayer(int Layer) { m_InterpolationLayer = Layer; }

	float GetBulrAlphaDecayRate() { return m_BulrAlphaDecayRate; }
	void SetBulrAlphaDecayRate(float Rate) { m_BulrAlphaDecayRate = Rate; }

	int GetFrameCount() { return m_FrameCount; }
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
